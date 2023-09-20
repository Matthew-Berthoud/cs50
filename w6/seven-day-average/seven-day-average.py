import csv
import requests

def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    new_cases = {}
    prev = {}

    for entry in reader:
        state = entry['state']
        cases = int(entry['cases'])

        if state not in new_cases: # first time
            new_cases[state] = [cases]
            prev[state] = cases
            continue

        if len(new_cases[state]) == 14:
            new_cases[state] = new_cases[state][1:]

        new = cases - prev[state]
        new_cases[state].append(new)

        prev[state] = new # for next time

    return new_cases


# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    for state in states:
        old_avg = round(sum(new_cases[state][0:7]) / 7)
        new_avg = round(sum(new_cases[state][7:]) / 7)
        diff = new_avg - old_avg
        try:
            pct_change = round(diff / old_avg * 100)
        except ZeroDivisionError:
            if new_avg > 0:
                pct_change = 100
            elif new_avg < 0:
                pct_change = -100
            else:
                pct_change = 0

        if pct_change >= 0:
            change = 'increase'
        else:
            change = 'decrease'

        print(f'{state} had a 7-day average of {new_avg} and a {change} of {pct_change}%.')


main()