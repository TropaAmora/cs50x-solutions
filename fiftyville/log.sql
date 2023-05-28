-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See the columns from crime_scene_reports
.schema crime_scene_reports

-- Look for the description of the report in July 28, 2021 on Hump. St
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

-- See columns in interviews
.schema interviews

-- See colums for bakery_security_logs
.schema bakery_security_logs

-- Look for information at the 10:15 am in that day, resulted in the license plates that are suspicious
-- license plates that left after 10
SELECT license_plate, activity, hour, minute FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10 AND minute > 14 AND activity = 'exit'
ORDER BY hour, minute;

-- license plates that were on the last query and arrived before that time
SELECT license_plate, activity, hour, minute, day FROM bakery_security_logs
WHERE hour < 11 AND minute < 16 AND activity = 'entrance'
AND license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28
    AND hour = 10 AND minute > 14 AND activity = 'exit'
    ORDER BY hour, minute
);

-- Eugene lead: Try to identify the account number
SELECT * FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';


-- Search from the airports in fiftyville (id, and abbreviation)
SELECT id, abbreviation FROM airports
WHERE city = 'Fiftyville';
-- abbreviation = 'CSF'

-- Search from the flights records flights departing from Fiftyville on that day, and limit the search for the first to leave
SELECT * FROM airports
JOIN flights ON flights.destination_airport_id = airports.id
WHERE flights.origin_airport_id IN (
    SELECT airports.id FROM airports
    WHERE city = 'Fiftyville'
)
AND flights.year = 2021
AND flights.month = 7
AND flights.day = 29
ORDER BY flights.hour, flights.minute
LIMIt 1;

-- Search the passengers from this flight
SELECT passport_number FROM passengers
JOIN flights ON flights.id = passengers.flight_id
WHERE flights.id IN (
    SELECT flights.id FROM flights
    JOIN airports ON airports.id = flights.origin_airport_id
    WHERE origin_airport_id IN (
        SELECT airports.id FROM airports
        WHERE airports.city = 'Fiftyville'
    )
    AND flights.year = 2021
    AND flights.month = 7
    AND flights.day = 29
    ORDER BY hour, minute
    LIMIT 1
);

-- Join the three leads together and search for the person present in all three and who received a call in that day
SELECT DISTINCT people.name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE people.passport_number IN (
    SELECT passport_number FROM passengers
    JOIN flights ON flights.id = passengers.flight_id
    WHERE flights.id IN (
        SELECT flights.id FROM flights
        WHERE flights.year = 2021
        AND flights.month = 7
        AND flights.day = 29
        ORDER BY hour, minute
        LIMIT 1
    )
)
AND atm_transactions.id IN (
    SELECT atm_transactions.id FROM atm_transactions
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw')
AND bakery_security_logs.license_plate IN (
    SELECT bakery_security_logs.license_plate FROM bakery_security_logs
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND hour = 10
    AND minute >= 15
    AND minute < 25
    AND activity = 'exit')
AND phone_calls.caller IN (
    SELECT phone_calls.caller FROM phone_calls
    WHERE phone_calls.year = 2021
    AND phone_calls.month = 7
    AND phone_calls.day = 28
    AND phone_calls.duration <= 60
);
-- Resulted in 'Bruce'

-- Search the name of the person who called Bruce
SELECT name FROM people
JOIN phone_calls ON phone_calls.receiver = people.phone_number
WHERE phone_calls.id IN (
    SELECT phone_calls.id FROM phone_calls
    JOIN people ON people.phone_number = phone_calls.caller
    WHERE people.phone_number IN (
        SELECT people.phone_number FROM people
        WHERE name = 'Bruce'
    )
    AND phone_calls.year = 2021
    AND phone_calls.month = 7
    AND phone_calls.day = 28
    AND phone_calls.duration <= 60
);
-- Resulted in 'Robin'


-- Search were did they escaped
SELECT full_name, abbreviation, city FROM airports
JOIN flights ON flights.destination_airport_id = airports.id
WHERE flights.origin_airport_id IN (
    SELECT airports.id FROM airports
    WHERE city = 'Fiftyville'
)
AND flights.year = 2021
AND flights.month = 7
AND flights.day = 29
ORDER BY flights.hour, flights.minute
LIMIt 1;
-- Resulted in New York City

-- Thief real answer
SELECT name FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flight_id = (SELECT id FROM flights
WHERE day = "29" AND month = "7" AND year = "2021" ORDER By hour, minute LIMIT 1)
INTERSECT SELECT DISTINCT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE day = "28" AND month = "7" AND year = "2021" AND transaction_type = "withdraw" AND atm_location = "Leggett Street"
INTERSECT SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE day = "28" AND month = "7" AND year = "2021" AND duration < "60"
INTERSECT SELECT name FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE day = "28" AND month = "7" AND year = "2021" AND hour = "10" AND minute >= "15" AND minute < "25" AND activity = "exit";


SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE day = "29" AND month = "7" AND year = "2021" ORDER BY hour,minute LIMIT 1);

SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE day = "28"
AND month = "7"
AND year = "2021"
AND duration < "60" AND caller = (SELECT phone_number FROM people WHERE name = "Bruce");