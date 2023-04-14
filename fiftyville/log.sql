-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See description of the crime
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = 'Humphrey Street';
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses
-- who were present at the time - each of their interview transcripts mentions the bakery.
-- Littering took place at 16:36. No known witnesses.

-- It is a good idea to look at the interviews
SELECT * FROM interviews
WHERE month = 7 and day = 28;
-- Ruth - Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
--        If you have security footage from teh bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene - I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
--          I was walking by the ATM on Leggett Street and saw the thief there withrdawing some money.
-- Raymond - As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say
--           that they were planning to take the earliest flgiht out of Fiftville tomorrow. The thief then asked the person on the other end of the
--           phone to purchase the flight ticket.

-- It is a good idea to look at the bakery's security record in the time frame as Ruth said
SELECT * FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25;
-- | 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |

-- So suspects are
SELECT * FROM people
WHERE license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25
);
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+---------+----------------+-----------------+---------------+

SELECT people.name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE bank_accounts.account_number IN
(
    SELECT account_number FROM atm_transactions
    WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
)
INTERSECT
SELECT name FROM people
WHERE license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25
);
-- +-------+
-- | name  |
-- +-------+
-- | Bruce |
-- | Diana |
-- | Iman  |
-- | Luca  |
-- +-------+


SELECT people.name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE bank_accounts.account_number IN
(
    SELECT account_number FROM atm_transactions
    WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
)
INTERSECT
SELECT name FROM people
WHERE license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25
)
INTERSECT
SELECT name FROM people
WHERE phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE month = 7 AND day = 28 AND duration < 60
)
INTERSECT
SELECT name FROM people
WHERE passport_number IN
(
    SELECT passport_number FROM passengers
    WHERE flight_id IN
    (
        SELECT id FROM flights
        WHERE month = 7 AND day = 29
        ORDER BY hour ASC, minute ASC
        LIMIT 1
    )
);

-- The thief is Bruce
SELECT airports.city FROM airports
JOIN flights ON flights.destination_airport_id =airports.id
JOIN passengers ON passengers.flight_id = flights.id
WHERE passengers.passport_number =
(
    SELECT passport_number FROM people
    WHERE name = 'Bruce'
);

-- The city they escaped to is New York City

SELECT name FROM people
WHERE phone_number =
(
    SELECT receiver FROM phone_calls
    WHERE caller =
    (
        SELECT phone_number FROM people
        WHERE name = 'Bruce' AND month = 7 AND day = 28 AND duration < 60
    )
);

-- Accomplice is Robin