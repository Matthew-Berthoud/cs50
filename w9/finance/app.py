import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")  # request.method is always "GET"
@login_required
def index():
    """Show portfolio of stocks"""
    user_info = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    user = user_info[0]["username"]
    cash = user_info[0]["cash"]

    stocks = db.execute("SELECT symbol, shares FROM holdings WHERE user_id = ?", session["user_id"])
    stock_total = 0
    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["value"] = stock["shares"] * stock["price"]
        stock_total += stock["value"]

    return render_template("index.html", user=user, stocks=stocks, cash=cash, stock_total=stock_total, total=cash + stock_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # input handling for stock symbol
        info = lookup(request.form.get("symbol"))
        if not info:
            return apology("invalid stock")

        # input handling for shares
        shares = request.form.get("shares")
        if not shares.isnumeric() or int(shares) < 1:
            return apology("enter positive whole number of shares")
        shares = int(shares)

        # make sure they have enough money to buy
        bal = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"] # this should just be an int
        newbal = bal - info["price"] * shares
        if newbal < 0:  # not enough money
            return apology("insufficient funds")

        # pay the money, log the trade
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newbal, session["user_id"])
        db.execute("INSERT INTO trades (user_id, symbol, shares, price) VALUES(?, ?, ?, ?)",
                   session["user_id"], info["symbol"], shares, info["price"])

        # update holdings (either add a new record or update current share count)
        row = db.execute("SELECT * FROM holdings WHERE user_id = ? AND symbol = ?", session["user_id"], info["symbol"])
        if row == []:  # you don't have this stock yet
            db.execute("INSERT INTO holdings (user_id, symbol, shares) VALUES(?, ?, ?)", session["user_id"], info["symbol"], shares)
        else:  # you have some shares of this stock already
            new_shares = row[0]["shares"] + shares
            db.execute("UPDATE holdings SET shares = ? WHERE user_id = ? AND symbol = ?",
                       new_shares, session["user_id"], info["symbol"])

        # feedback for the user
        name = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["username"]
        plural = ''
        if shares > 1:
            plural = 's'
        flash(f'{name} bought {shares} share{plural} of {info["symbol"]} at {usd(info["price"])} per share')

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    my_trades = db.execute("SELECT symbol, price, shares, timestamp FROM trades WHERE user_id = ?", session["user_id"])
    return render_template("history.html", trades=my_trades)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        info = lookup(request.form.get("symbol"))
        if not info:
            return apology("invalid ticker symbol")
        return render_template("quoted.html", symbol=info["symbol"], price=info["price"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    session.clear()
    if request.method == "POST":
        username = request.form.get("username")
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        p1 = request.form.get("password")
        p2 = request.form.get("confirmation")

        # make sure they type a username
        if not username:
            return apology("must provide username")

        # make sure username doesn't yet exist
        elif len(rows) > 0:
            return apology("username already exists")

        # Ensure password was submitted, and they match
        elif not p1:
            return apology("must provide password")
        elif p1 != p2:
            return apology("passwords do not match")

        hash = generate_password_hash(p1)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)
        user_record = db.execute("SELECT * FROM users WHERE username = ?", username)[0]  # dict
        session["user_id"] = user_record["id"]

        flash("Registration successful")
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    my_stocks = db.execute("SELECT * FROM holdings WHERE user_id = ?", session["user_id"])
    stocks = [stock["symbol"] for stock in my_stocks if stock["shares"] > 0]
    # if this list comprehension doesn't work,
    # stocks = []
    # for stock in my_stocks:
    #     if stock["shares"] > 0:
    #         stocks += [stock["symbol"]]

    if request.method == "POST":
        # validity checks
        stock_to_sell = request.form.get("symbol")
        if not stock_to_sell:
            return apology("invalid stock")
        row = db.execute("SELECT * FROM holdings WHERE user_id = ? AND symbol = ?", session["user_id"], stock_to_sell)
        if len(row) != 1:  # delete once I know it doesn't happen
            return apology("this should never happen")
        shares_held = row[0]["shares"]
        if shares_held == 0:
            return apology("no shares to sell")
        shares_to_sell = int(request.form.get("shares"))
        if shares_held < shares_to_sell or shares_to_sell < 1:
            return apology("invalid number of shares")

        # process the sale
        price = lookup(stock_to_sell)["price"]
        revenue = price * shares_to_sell

        db.execute("UPDATE holdings SET shares = shares - ? WHERE user_id = ? AND symbol = ?",
                   shares_to_sell, session["user_id"], stock_to_sell)
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", revenue, session["user_id"])
        db.execute("INSERT INTO trades (user_id, symbol, shares, price) VALUES(?, ?, ?, ?)", session["user_id"],
                   stock_to_sell, -shares_to_sell, price)

        # feedback for the user
        name = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["username"]
        plural = ''
        if shares_to_sell > 1:
            plural = 's'
        flash(f'{name} sold {shares_to_sell} share{plural} of {stock_to_sell} at {usd(price)} per share')

        return redirect("/")
    else:
        return render_template("sell.html", stocks=stocks)


@app.route("/settings")
@login_required
def settings():
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]
    return render_template("settings.html", user=user)


@app.route("/settings/username", methods=["GET", "POST"])
@login_required
def change_username():
    if request.method == "POST":
        new = request.form.get("new")
        old = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["username"]

        # check that it doesn't already exist
        # unless it's the same as your old one
        rows = db.execute("SELECT * FROM users WHERE username = ?", new)
        if len(rows) > 0 and new != old:
            return apology("username already in use")

        # make the change
        db.execute("UPDATE users SET username = ? WHERE id = ?", new, session["user_id"])

        # feedback
        flash(f'Username changed from "{old}" to "{new}"')
        return redirect("/settings")
    else:
        return render_template("username.html", user=db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0])


@app.route("/settings/password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        old = request.form.get("old")
        new = request.form.get("new")
        conf = request.form.get("confirmation")

        # check that hash(old) is correct
        real_old = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["hash"]
        if not check_password_hash(real_old, old):
            return apology("old password incorrect")

        # check that hash(new) == hash(conf)
        if new != conf:
            return apology("new passwords do not match")

        # change password
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new), session["user_id"])

        flash("Password changed")
        return redirect("/settings")
    else:
        return render_template("password.html", user=db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0])


@app.route("/settings/balance", methods=["GET", "POST"])
@login_required
def change_balance():
    if request.method == "POST":
        deposit = request.form.get("deposit")
        withdrawal = request.form.get("withdrawal")

        # most error checking handled by min, max, and step attributes in the template
        # check that one and only one of the fields has an entry
        if not (bool(deposit) ^ bool(withdrawal)):
            return apology("enter dollar amount in exactly one field")

        if bool(deposit):
            update = float(deposit)
        if bool(withdrawal):
            update = -1 * float(withdrawal)

        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", update, session["user_id"])

        flash("Balance updated")
        return redirect("/settings")
    else:
        return render_template("balance.html", user=db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0])
    

"""
tables I made:

CREATE TABLE trades (
    trade_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares NUMERIC NOT NULL,
    price NUMERIC NOT NULL,
    timestamp DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE holdings (
    holding_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares NUMERIC NOT NULL
);


to reset stuff, but not delete accounts:
DELETE FROM trades;
DELETE FROM holdings;
UPDATE users SET cash = 10000.00;

check on things:
SELECT id, username, cash FROM users;
SELECT * FROM holdings;
SELECT * FROM trades;
"""