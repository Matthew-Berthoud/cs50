-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Things I know:
    -- Crime took place on July 28
    -- Crime took place on Humphrey Street

-- Things to identify
    -- Who is the thief
    -- Where they escaped to
    -- Who is the accomplice

-- Start with crime scene reports, drop everything that isn't July 28 on Humph
SELECT description, year
  FROM crime_scene_reports
 WHERE month = 7 AND day = 28
   AND street = 'Humphrey Street'; -- This is from the video

-- RESULTS:
-- 10:15am
    SELECT activity, license_plate, id, minute FROM bakery_security_logs
     WHERE month = 7 AND day = 28 AND year = 2021 AND hour = 10 AND minute <= 25; -- exact minute is too specific
    -- RESULTS:
    -- Not a single car entered and exited in this time...
    -- more on this later
-- Humphrey Street bakery
-- 3 witnesses, all mentioning bakery in transcripts
    SELECT id, name, transcript FROM interviews
     WHERE month = 7 AND day = 28 AND year = 2021
       AND transcript LIKE '%bakery%';
    -- WITNESSES:
        -- 161 Ruth
            -- Thief left bakery within ten minutes of the theft
        -- 162 Eugene
            -- Thief withdrew money from ATM on Leggett Street
                -- earlier in the morning before Eugene arrived at bakery
        -- 163 Raymond
            -- As thief was leaving bakery, called accomplice for under a minute
            -- Said they planned to take the earliest flight out of Fiftyville tmrw
            -- Asked ACCOMPLICE to purchase flight ticket

-- Who is the thief?
-- Cross-reference Ruth's with Eugene's info
    -- left bakery within ten minutes (minutes < 26)
    -- atm withdrawal on Leggett Street before 10:15

SELECT * FROM people

WHERE id IN
(SELECT person_id FROM bank_accounts

WHERE account_number IN
(SELECT account_number FROM atm_transactions
WHERE month = 7 AND day = 28 AND year = 2021
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw')) -- Eugene's interview

AND license_plate IN
(SELECT license_plate FROM bakery_security_logs -- Ruth
WHERE month = 7 AND day = 28 AND year = 2021 AND hour = 10 AND minute <= 25 AND minute >= 15
AND activity = 'exit') -- Narrowed down to four

AND phone_number IN
(SELECT caller FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2021
AND duration < 60 -- from Raymond

-- Okay how the hell did Diana not witness Bruce plan and commit this crime
-- Or vice versa

-- Let's check if their accomplice (reciever)
-- has a plane ticket for the first flight

    AND caller IN
    (SELECT phone_number FROM people

    WHERE passport_number IN
    (SELECT passport_number FROM passengers

    WHERE flight_id IN
    (SELECT id FROM flights
    WHERE month = 7 AND day = 29 AND year = 2021)))); -- flight leaves tomorrow

-- no way it's still Diana and Bruce man...

-- Okay I need to find out what city we're in so I can find what airport
-- Then I can see what the earliest flight is the following morning
-- No city info in crime report
-- I just need to check what two flights they're on

-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+

SELECT *
FROM flights
WHERE id IN
(SELECT flight_id FROM passengers
WHERE (passport_number = 3592750733 OR passport_number = 5773159633)
AND month = 7 AND day = 29 AND year = 2021);

-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+
-- | 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      |
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+

-- Same origin and the second one leaves earlier

SELECT * FROM airports WHERE id = 8;

-- +----+--------------+-----------------------------+------------+
-- | id | abbreviation |          full_name          |    city    |
-- +----+--------------+-----------------------------+------------+
-- | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- +----+--------------+-----------------------------+------------+

SELECT * FROM people
WHERE (id = 514354 OR id = 686048) -- diana or bruce
AND passport_number IN
(SELECT passport_number FROM passengers
WHERE flight_id = 36); -- THE CULPRIT

-- IT WAS YOU, BRUCE
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+

SELECT * FROM phone_calls
WHERE month = 7 AND day = 28 AND year = 2021
AND duration < 60
AND caller = '(367) 555-5533';

SELECT * FROM people -- Other end of the line during the phone call
WHERE phone_number = '(375) 555-8161'; -- THE ACCOMPLICE

-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+

SELECT * FROM airports WHERE id =
(SELECT destination_airport_id FROM flights WHERE id = 36); -- where they escaped

-- +----+--------------+-------------------+---------------+
-- | id | abbreviation |     full_name     |     city      |
-- +----+--------------+-------------------+---------------+
-- | 4  | LGA          | LaGuardia Airport | New York City |
-- +----+--------------+-------------------+---------------+