SELECT movies.title, ratings.rating FROM movies
JOIN ratings ON ratings.movie_id = movies.id
WHERE movies.year = 2010
EXCEPT
SELECT movies.title, ratings.rating FROM movies
JOIN ratings ON ratings.movie_id = movies.id
WHERE ratings.rating = NULL
ORDER BY ratings.rating DESC, movies.title;