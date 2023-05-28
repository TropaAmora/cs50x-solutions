SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = 'Johnny Depp'
AND title IN (SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = 'Helena Bonham Carter')

--
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

SELECT DISTINCT people.name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
JOIN phone_calls ON phone_calls.receiver = people.phone_number
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
AND phone_calls.receiver IN (
    SELECT phone_calls.receiver FROM phone_calls
    WHERE phone_calls.year = 2021
    AND phone_calls.month = 7
    AND phone_calls.day = 28
    AND phone_calls.duration <= 60
);