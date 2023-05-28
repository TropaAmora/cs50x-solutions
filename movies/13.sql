SELECT DISTINCT name FROM people
JOIN stars ON stars.person_id = people.id
WHERE people.id IN (
    SELECT DISTINCT person_id FROM stars
    JOIN people ON people.id = stars.person_id
    WHERE movie_id IN (
        SELECT movie_id FROM stars
        JOIN people ON people.id = stars.person_id
        WHERE people.name = 'Kevin Bacon' AND people.birth = 1958
    )
) AND people.name != 'Kevin Bacon';