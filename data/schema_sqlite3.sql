create table game (
	id integer primary key,
	gametime int,
);

create table player (
	id integer primary key,
	player_name char(100)
);

create table game_player (
	id integer primary key,
	game_id int,
	player_id int,
	player_score int
);

create table player_round (
	id integer primary key,
	game_player_id int,
	points int,
	air_5 int,
	air_3 int,
	air_1 int,
	brb_5 int,
	brb_3 int,
	imiss_5 int,
	imiss_3 int
);
