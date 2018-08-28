CREATE SCHEMA "Default";

CREATE SCHEMA information_schema;

CREATE SCHEMA pg_catalog;

CREATE SCHEMA "public";

CREATE TABLE "Missions" ( 
	player_id            bigint   ,
	mission_id           integer  NOT NULL ,
	mission_progress     integer[]   ,
	mission_status       char(1)   ,
	CONSTRAINT unq_missions_player_id UNIQUE ( player_id ) 
 );

CREATE TABLE "Statistics" ( 
	player_id            bigint  NOT NULL ,
	statistic_id         integer  NOT NULL ,
	statistic_value      integer  NOT NULL ,
	CONSTRAINT "Pk_Statistics_playerID" PRIMARY KEY ( player_id )
 );

CREATE TABLE global_numberic_variables ( 
	name                 varchar(100)  NOT NULL ,
	"value"              bigint   ,
	CONSTRAINT pk_global_numberic_variables_name PRIMARY KEY ( name )
 );

CREATE TABLE unlocked_emotes ( 
	player_id            bigint  NOT NULL ,
	emote_id             integer   ,
	CONSTRAINT pk_unlocked_emotes_player_id PRIMARY KEY ( player_id )
 );

CREATE TABLE characters ( 
	player_id            bigint  NOT NULL ,
	CONSTRAINT pk_characters_player_id PRIMARY KEY ( player_id ),
	CONSTRAINT fk_characters_missions FOREIGN KEY ( player_id ) REFERENCES "Missions"( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_characters_statistics FOREIGN KEY ( player_id ) REFERENCES "Statistics"( player_id )  ,
	CONSTRAINT fk_characters_unlocked_emotes FOREIGN KEY ( player_id ) REFERENCES unlocked_emotes( player_id )  
 );

COMMENT ON COLUMN characters.player_id IS 'WIP!';

CREATE TABLE character_map ( 
	account_id           integer  NOT NULL ,
	charactera_id        bigint   ,
	characterb_id        bigint   ,
	characterc_id        bigint   ,
	characterd_id        bigint   ,
	CONSTRAINT pk_character_map_account_id PRIMARY KEY ( account_id ),
	CONSTRAINT fk_character_map_characters_a FOREIGN KEY ( charactera_id ) REFERENCES characters( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_character_map_characters_b FOREIGN KEY ( characterb_id ) REFERENCES characters( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_character_map_characters_c FOREIGN KEY ( characterc_id ) REFERENCES characters( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_character_map_characters_d FOREIGN KEY ( characterd_id ) REFERENCES characters( player_id ) ON DELETE CASCADE 
 );

CREATE INDEX idx_character_map_charactera_id ON character_map ( charactera_id );

CREATE INDEX idx_character_map_characterb_id ON character_map ( characterb_id );

CREATE INDEX idx_character_map_characterc_id ON character_map ( characterc_id );

CREATE INDEX idx_character_map_characterd_id ON character_map ( characterd_id );

CREATE TABLE accounts ( 
	lur_uuid             uuid  NOT NULL ,
	account_id           integer  NOT NULL ,
	last_login           timestamp   ,
	ingame_session_key   uuid   ,
	service_session_key  uuid   ,
	CONSTRAINT pk_accounts_lur_uuid PRIMARY KEY ( lur_uuid ),
	CONSTRAINT fk_accounts_character_map FOREIGN KEY ( account_id ) REFERENCES character_map( account_id ) ON DELETE CASCADE 
 );

CREATE INDEX idx_accounts_account_id ON accounts ( account_id );

CREATE TABLE "public"."Missions" ( 
	player_id            bigint   ,
	mission_id           integer  NOT NULL ,
	mission_progress     integer[]   ,
	mission_status       char(1)   ,
	CONSTRAINT unq_missions_player_id UNIQUE ( player_id ) 
 );

CREATE TABLE "public"."Statistics" ( 
	player_id            bigint  NOT NULL ,
	statistic_id         integer  NOT NULL ,
	statistic_value      integer  NOT NULL ,
	CONSTRAINT "Pk_Statistics_playerID" PRIMARY KEY ( player_id )
 );

CREATE TABLE "public".global_numberic_variables ( 
	name                 varchar(100)  NOT NULL ,
	"value"              bigint   ,
	CONSTRAINT pk_global_numberic_variables_name PRIMARY KEY ( name )
 );

CREATE TABLE "public".unlocked_emotes ( 
	player_id            bigint  NOT NULL ,
	emote_id             integer   ,
	CONSTRAINT pk_unlocked_emotes_player_id PRIMARY KEY ( player_id )
 );

CREATE TABLE "public".characters ( 
	player_id            bigint  NOT NULL ,
	CONSTRAINT pk_characters_player_id PRIMARY KEY ( player_id ),
	CONSTRAINT fk_characters_missions FOREIGN KEY ( player_id ) REFERENCES "public"."Missions"( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_characters_statistics FOREIGN KEY ( player_id ) REFERENCES "public"."Statistics"( player_id )  ,
	CONSTRAINT fk_characters_unlocked_emotes FOREIGN KEY ( player_id ) REFERENCES "public".unlocked_emotes( player_id )  
 );

COMMENT ON COLUMN "public".characters.player_id IS 'WIP!';

CREATE TABLE "public".character_map ( 
	account_id           integer  NOT NULL ,
	charactera_id        bigint   ,
	characterb_id        bigint   ,
	characterc_id        bigint   ,
	characterd_id        bigint   ,
	CONSTRAINT pk_character_map_account_id PRIMARY KEY ( account_id ),
	CONSTRAINT fk_character_map_characters_a FOREIGN KEY ( charactera_id ) REFERENCES "public".characters( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_character_map_characters_b FOREIGN KEY ( characterb_id ) REFERENCES "public".characters( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_character_map_characters_c FOREIGN KEY ( characterc_id ) REFERENCES "public".characters( player_id ) ON DELETE CASCADE ,
	CONSTRAINT fk_character_map_characters_d FOREIGN KEY ( characterd_id ) REFERENCES "public".characters( player_id ) ON DELETE CASCADE 
 );

CREATE INDEX idx_character_map_charactera_id ON "public".character_map ( charactera_id );

CREATE INDEX idx_character_map_characterb_id ON "public".character_map ( characterb_id );

CREATE INDEX idx_character_map_characterc_id ON "public".character_map ( characterc_id );

CREATE INDEX idx_character_map_characterd_id ON "public".character_map ( characterd_id );

CREATE TABLE "public".accounts ( 
	lur_uuid             uuid  NOT NULL ,
	account_id           integer  NOT NULL ,
	last_login           timestamp   ,
	ingame_session_key   uuid   ,
	service_session_key  uuid   ,
	CONSTRAINT pk_accounts_lur_uuid PRIMARY KEY ( lur_uuid ),
	CONSTRAINT fk_accounts_character_map FOREIGN KEY ( account_id ) REFERENCES "public".character_map( account_id ) ON DELETE CASCADE 
 );

CREATE INDEX idx_accounts_account_id ON "public".accounts ( account_id );

create function _pg_char_max_length SELECT
  CASE WHEN $2 = -1 /* default typmod */
       THEN null
       WHEN $1 IN (1042, 1043) /* char, varchar */
       THEN $2 - 4
       WHEN $1 IN (1560, 1562) /* bit, varbit */
       THEN $2
       ELSE null
  END

create function _pg_char_octet_length SELECT
  CASE WHEN $1 IN (25, 1042, 1043) /* text, char, varchar */
       THEN CASE WHEN $2 = -1 /* default typmod */
                 THEN CAST(2^30 AS integer)
                 ELSE information_schema._pg_char_max_length($1, $2) *
                      pg_catalog.pg_encoding_max_length((SELECT encoding FROM pg_catalog.pg_database WHERE datname = pg_catalog.current_database()))
            END
       ELSE null
  END

create function _pg_datetime_precision SELECT
  CASE WHEN $1 IN (1082) /* date */
           THEN 0
       WHEN $1 IN (1083, 1114, 1184, 1266) /* time, timestamp, same + tz */
           THEN CASE WHEN $2 < 0 THEN 6 ELSE $2 END
       WHEN $1 IN (1186) /* interval */
           THEN CASE WHEN $2 < 0 OR $2 & 65535 = 65535 THEN 6 ELSE $2 & 65535 END
       ELSE null
  END

create function _pg_expandarray select $1[s], s - pg_catalog.array_lower($1,1) + 1
        from pg_catalog.generate_series(pg_catalog.array_lower($1,1),
                                        pg_catalog.array_upper($1,1),
                                        1) as g(s)

create function _pg_index_position 
SELECT (ss.a).n FROM
  (SELECT information_schema._pg_expandarray(indkey) AS a
   FROM pg_catalog.pg_index WHERE indexrelid = $1) ss
  WHERE (ss.a).x = $2;

create function _pg_interval_type SELECT
  CASE WHEN $1 IN (1186) /* interval */
           THEN upper(substring(format_type($1, $2) from 'interval[()0-9]* #"%#"' for '#'))
       ELSE null
  END

create function _pg_keysequal select $1 operator(pg_catalog.<@) $2 and $2 operator(pg_catalog.<@) $1

create function _pg_numeric_precision SELECT
  CASE $1
         WHEN 21 /*int2*/ THEN 16
         WHEN 23 /*int4*/ THEN 32
         WHEN 20 /*int8*/ THEN 64
         WHEN 1700 /*numeric*/ THEN
              CASE WHEN $2 = -1
                   THEN null
                   ELSE (($2 - 4) >> 16) & 65535
                   END
         WHEN 700 /*float4*/ THEN 24 /*FLT_MANT_DIG*/
         WHEN 701 /*float8*/ THEN 53 /*DBL_MANT_DIG*/
         ELSE null
  END

create function _pg_numeric_precision_radix SELECT
  CASE WHEN $1 IN (21, 23, 20, 700, 701) THEN 2
       WHEN $1 IN (1700) THEN 10
       ELSE null
  END

create function _pg_numeric_scale SELECT
  CASE WHEN $1 IN (21, 23, 20) THEN 0
       WHEN $1 IN (1700) THEN
            CASE WHEN $2 = -1
                 THEN null
                 ELSE ($2 - 4) & 65535
                 END
       ELSE null
  END

create function _pg_truetypid SELECT CASE WHEN $2.typtype = 'd' THEN $2.typbasetype ELSE $1.atttypid END

create function _pg_truetypmod SELECT CASE WHEN $2.typtype = 'd' THEN $2.typtypmod ELSE $1.atttypmod END

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_cascade_del"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_cascade_del$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_cascade_upd"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_cascade_upd$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_check_ins"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_check_ins$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_check_upd"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_check_upd$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_noaction_del"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_noaction_del$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_noaction_upd"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_noaction_upd$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_restrict_del"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_restrict_del$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_restrict_upd"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_restrict_upd$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_setdefault_del"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_setdefault_del$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_setdefault_upd"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_setdefault_upd$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_setnull_del"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_setnull_del$function$

CREATE OR REPLACE FUNCTION pg_catalog."RI_FKey_setnull_upd"()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$RI_FKey_setnull_upd$function$

CREATE OR REPLACE FUNCTION pg_catalog.abbrev(cidr)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidr_abbrev$function$

CREATE OR REPLACE FUNCTION pg_catalog.abbrev(inet)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_abbrev$function$

CREATE OR REPLACE FUNCTION pg_catalog.abs(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.abs(smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.abs(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.abs(bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.abs(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.abs(real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstime(timestamp without time zone)
 RETURNS abstime
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_abstime$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstime(timestamp with time zone)
 RETURNS abstime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptz_abstime$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimeeq(abstime, abstime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$abstimeeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimege(abstime, abstime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$abstimege$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimegt(abstime, abstime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$abstimegt$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimein(cstring)
 RETURNS abstime
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$abstimein$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimele(abstime, abstime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$abstimele$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimelt(abstime, abstime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$abstimelt$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimene(abstime, abstime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$abstimene$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimeout(abstime)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$abstimeout$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimerecv(internal)
 RETURNS abstime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$abstimerecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.abstimesend(abstime)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$abstimesend$function$

CREATE OR REPLACE FUNCTION pg_catalog.aclcontains(aclitem[], aclitem)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$aclcontains$function$

CREATE OR REPLACE FUNCTION pg_catalog.acldefault("char", oid)
 RETURNS aclitem[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$acldefault_sql$function$

CREATE OR REPLACE FUNCTION pg_catalog.aclexplode(acl aclitem[], OUT grantor oid, OUT grantee oid, OUT privilege_type text, OUT is_grantable boolean)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT ROWS 10
AS $function$aclexplode$function$

CREATE OR REPLACE FUNCTION pg_catalog.aclinsert(aclitem[], aclitem)
 RETURNS aclitem[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$aclinsert$function$

CREATE OR REPLACE FUNCTION pg_catalog.aclitemeq(aclitem, aclitem)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$aclitem_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.aclitemin(cstring)
 RETURNS aclitem
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$aclitemin$function$

CREATE OR REPLACE FUNCTION pg_catalog.aclitemout(aclitem)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$aclitemout$function$

CREATE OR REPLACE FUNCTION pg_catalog.aclremove(aclitem[], aclitem)
 RETURNS aclitem[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$aclremove$function$

CREATE OR REPLACE FUNCTION pg_catalog.acos(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dacos$function$

CREATE OR REPLACE FUNCTION pg_catalog.acosd(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dacosd$function$

CREATE OR REPLACE FUNCTION pg_catalog.age(timestamp without time zone)
 RETURNS interval
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.age(cast(current_date as timestamp without time zone), $1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.age(timestamp without time zone, timestamp without time zone)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_age$function$

CREATE OR REPLACE FUNCTION pg_catalog.age(timestamp with time zone)
 RETURNS interval
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.age(cast(current_date as timestamp with time zone), $1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.age(timestamp with time zone, timestamp with time zone)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptz_age$function$

CREATE OR REPLACE FUNCTION pg_catalog.age(xid)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$xid_age$function$

CREATE OR REPLACE FUNCTION pg_catalog.amvalidate(oid)
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$amvalidate$function$

CREATE OR REPLACE FUNCTION pg_catalog.any_in(cstring)
 RETURNS "any"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$any_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.any_out("any")
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$any_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyarray_in(cstring)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$anyarray_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyarray_out(anyarray)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$anyarray_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyarray_recv(internal)
 RETURNS anyarray
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$anyarray_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyarray_send(anyarray)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$anyarray_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyelement_in(cstring)
 RETURNS anyelement
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$anyelement_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyelement_out(anyelement)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$anyelement_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyenum_in(cstring)
 RETURNS anyenum
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$anyenum_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyenum_out(anyenum)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$anyenum_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.anynonarray_in(cstring)
 RETURNS anynonarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$anynonarray_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.anynonarray_out(anynonarray)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$anynonarray_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyrange_in(cstring, oid, integer)
 RETURNS anyrange
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$anyrange_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.anyrange_out(anyrange)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$anyrange_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.anytextcat(anynonarray, text)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select $1::pg_catalog.text || $2$function$

CREATE OR REPLACE FUNCTION pg_catalog.area(path)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_area$function$

CREATE OR REPLACE FUNCTION pg_catalog.area(box)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_area$function$

CREATE OR REPLACE FUNCTION pg_catalog.area(circle)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_area$function$

CREATE OR REPLACE FUNCTION pg_catalog.areajoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$areajoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.areasel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$areasel$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_agg_array_finalfn(internal, anyarray)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_agg_array_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_agg_array_transfn(internal, anyarray)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_agg_array_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_agg_finalfn(internal, anynonarray)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_agg_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_agg_transfn(internal, anynonarray)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_agg_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_append(anyarray, anyelement)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_append$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_cat(anyarray, anyarray)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_cat$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_dims(anyarray)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_dims$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_eq(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_fill(anyelement, integer[], integer[])
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_fill_with_lower_bounds$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_fill(anyelement, integer[])
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_fill$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_ge(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_gt(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_in(cstring, oid, integer)
 RETURNS anyarray
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_larger(anyarray, anyarray)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_le(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_length(anyarray, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_lower(anyarray, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_lower$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_lt(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_ndims(anyarray)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_ndims$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_ne(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_out(anyarray)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_position(anyarray, anyelement, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_position_start$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_position(anyarray, anyelement)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_position$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_positions(anyarray, anyelement)
 RETURNS integer[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_positions$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_prepend(anyelement, anyarray)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_prepend$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_recv(internal, oid, integer)
 RETURNS anyarray
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_remove(anyarray, anyelement)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_remove$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_replace(anyarray, anyelement, anyelement)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$array_replace$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_send(anyarray)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_smaller(anyarray, anyarray)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_to_json(anyarray, boolean)
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_to_json_pretty$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_to_json(anyarray)
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_to_json$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_to_string(anyarray, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_to_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_to_string(anyarray, text, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$array_to_text_null$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_to_tsvector(text[])
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_to_tsvector$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_typanalyze(internal)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$array_typanalyze$function$

CREATE OR REPLACE FUNCTION pg_catalog.array_upper(anyarray, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_upper$function$

CREATE OR REPLACE FUNCTION pg_catalog.arraycontained(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$arraycontained$function$

CREATE OR REPLACE FUNCTION pg_catalog.arraycontains(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$arraycontains$function$

CREATE OR REPLACE FUNCTION pg_catalog.arraycontjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$arraycontjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.arraycontsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$arraycontsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.arrayoverlap(anyarray, anyarray)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$arrayoverlap$function$

CREATE OR REPLACE FUNCTION pg_catalog.ascii(text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ascii$function$

CREATE OR REPLACE FUNCTION pg_catalog.ascii_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/ascii_and_mic', $function$ascii_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.ascii_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_ascii', $function$ascii_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.asin(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dasin$function$

CREATE OR REPLACE FUNCTION pg_catalog.asind(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dasind$function$

CREATE OR REPLACE FUNCTION pg_catalog.atan(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datan$function$

CREATE OR REPLACE FUNCTION pg_catalog.atan2(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datan2$function$

CREATE OR REPLACE FUNCTION pg_catalog.atan2d(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datan2d$function$

CREATE OR REPLACE FUNCTION pg_catalog.atand(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datand$function$

CREATE OR REPLACE FUNCTION pg_catalog.bernoulli(internal)
 RETURNS tsm_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$tsm_bernoulli_handler$function$

CREATE OR REPLACE FUNCTION pg_catalog.big5_to_euc_tw(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_tw_and_big5', $function$big5_to_euc_tw$function$

CREATE OR REPLACE FUNCTION pg_catalog.big5_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_tw_and_big5', $function$big5_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.big5_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_big5', $function$big5_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_create_empty_extension(text, text, boolean, text, oid[], text[], text[])
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED
AS $function$binary_upgrade_create_empty_extension$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_array_pg_type_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_array_pg_type_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_heap_pg_class_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_heap_pg_class_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_index_pg_class_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_index_pg_class_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_pg_authid_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_pg_authid_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_pg_enum_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_pg_enum_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_pg_type_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_pg_type_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_toast_pg_class_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_toast_pg_class_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_next_toast_pg_type_oid(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_next_toast_pg_type_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.binary_upgrade_set_record_init_privs(boolean)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$binary_upgrade_set_record_init_privs$function$

CREATE OR REPLACE FUNCTION pg_catalog."bit"(bigint, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitfromint8$function$

CREATE OR REPLACE FUNCTION pg_catalog."bit"(bit, integer, boolean)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bit$function$

CREATE OR REPLACE FUNCTION pg_catalog."bit"(integer, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitfromint4$function$

CREATE OR REPLACE FUNCTION pg_catalog.bit_in(cstring, oid, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bit_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.bit_length(bit)
 RETURNS integer
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.length($1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.bit_length(text)
 RETURNS integer
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.octet_length($1) * 8$function$

CREATE OR REPLACE FUNCTION pg_catalog.bit_length(bytea)
 RETURNS integer
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.octet_length($1) * 8$function$

CREATE OR REPLACE FUNCTION pg_catalog.bit_out(bit)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bit_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.bit_recv(internal, oid, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bit_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.bit_send(bit)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bit_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitand(bit, bit)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bit_and$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitcat(bit varying, bit varying)
 RETURNS bit varying
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitcat$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitcmp(bit, bit)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.biteq(bit, bit)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$biteq$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitge(bit, bit)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitge$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitgt(bit, bit)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitgt$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitle(bit, bit)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitle$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitlt(bit, bit)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitne(bit, bit)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitne$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitnot(bit)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitnot$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitor(bit, bit)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bit_or$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitshiftleft(bit, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitshiftleft$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitshiftright(bit, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitshiftright$function$

CREATE OR REPLACE FUNCTION pg_catalog.bittypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bittypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.bittypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bittypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.bitxor(bit, bit)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitxor$function$

CREATE OR REPLACE FUNCTION pg_catalog.bool(integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4_bool$function$

CREATE OR REPLACE FUNCTION pg_catalog.bool_accum(internal, boolean)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$bool_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.bool_accum_inv(internal, boolean)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$bool_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.bool_alltrue(internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bool_alltrue$function$

CREATE OR REPLACE FUNCTION pg_catalog.bool_anytrue(internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bool_anytrue$function$

CREATE OR REPLACE FUNCTION pg_catalog.booland_statefunc(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$booland_statefunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.booleq(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$booleq$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolge(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$boolge$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolgt(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$boolgt$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolin(cstring)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$boolin$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolle(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$boolle$function$

CREATE OR REPLACE FUNCTION pg_catalog.boollt(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$boollt$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolne(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$boolne$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolor_statefunc(boolean, boolean)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$boolor_statefunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolout(boolean)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$boolout$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolrecv(internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$boolrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.boolsend(boolean)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$boolsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.bound_box(box, box)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$boxes_bound_box$function$

CREATE OR REPLACE FUNCTION pg_catalog.box(point)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_box$function$

CREATE OR REPLACE FUNCTION pg_catalog.box(circle)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_box$function$

CREATE OR REPLACE FUNCTION pg_catalog.box(polygon)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_box$function$

CREATE OR REPLACE FUNCTION pg_catalog.box(point, point)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$points_box$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_above(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_above$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_above_eq(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_above_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_add(box, point)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_add$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_below(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_below$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_below_eq(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_below_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_center(box)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_contain(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_contain$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_contain_pt(box, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_contain_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_contained(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_contained$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_distance(box, box)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_div(box, point)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_div$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_eq(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_ge(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_gt(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_in(cstring)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_intersect(box, box)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_intersect$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_le(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_left(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_left$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_lt(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_mul(box, point)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_out(box)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_overabove(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_overabove$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_overbelow(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_overbelow$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_overlap(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_overlap$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_overleft(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_overleft$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_overright(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_overright$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_recv(internal)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_right(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_right$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_same(box, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_send(box)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.box_sub(box, point)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_sub$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar("char")
 RETURNS character
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$char_bpchar$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar(character, integer, boolean)
 RETURNS character
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar(name)
 RETURNS character
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$name_bpchar$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar_larger(character, character)
 RETURNS character
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar_pattern_ge(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_pattern_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar_pattern_gt(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_pattern_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar_pattern_le(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_pattern_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar_pattern_lt(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_pattern_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar_smaller(character, character)
 RETURNS character
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchar_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharcmp(character, character)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchareq(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bpchareq$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharge(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharge$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchargt(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchargt$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchariclike(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharicnlike(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticnlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharicregexeq(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticregexeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharicregexne(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticregexne$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharin(cstring, oid, integer)
 RETURNS character
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharin$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharle(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharle$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharlike(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharlt(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharne(character, character)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bpcharne$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharnlike(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textnlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharout(character)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharout$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharrecv(internal, oid, integer)
 RETURNS character
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$bpcharrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharregexeq(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textregexeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharregexne(character, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textregexne$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpcharsend(character)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$bpcharsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchartypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchartypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.bpchartypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchartypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_inclusion_add_value(internal, internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_inclusion_add_value$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_inclusion_consistent(internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_inclusion_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_inclusion_opcinfo(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_inclusion_opcinfo$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_inclusion_union(internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_inclusion_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_minmax_add_value(internal, internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_minmax_add_value$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_minmax_consistent(internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_minmax_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_minmax_opcinfo(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_minmax_opcinfo$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_minmax_union(internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$brin_minmax_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.brin_summarize_new_values(regclass)
 RETURNS integer
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$brin_summarize_new_values$function$

CREATE OR REPLACE FUNCTION pg_catalog.brinhandler(internal)
 RETURNS index_am_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$brinhandler$function$

CREATE OR REPLACE FUNCTION pg_catalog.broadcast(inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_broadcast$function$

CREATE OR REPLACE FUNCTION pg_catalog.btabstimecmp(abstime, abstime)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btabstimecmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btarraycmp(anyarray, anyarray)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btarraycmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btboolcmp(boolean, boolean)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btboolcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btbpchar_pattern_cmp(character, character)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btbpchar_pattern_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btbpchar_pattern_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btbpchar_pattern_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.btcharcmp("char", "char")
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btcharcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btfloat48cmp(real, double precision)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btfloat48cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btfloat4cmp(real, real)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btfloat4cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btfloat4sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btfloat4sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.btfloat84cmp(double precision, real)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btfloat84cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btfloat8cmp(double precision, double precision)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btfloat8cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btfloat8sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btfloat8sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.bthandler(internal)
 RETURNS index_am_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$bthandler$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint24cmp(smallint, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint24cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint28cmp(smallint, bigint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint28cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint2cmp(smallint, smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint2cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint2sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint2sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint42cmp(integer, smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint42cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint48cmp(integer, bigint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint48cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint4cmp(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint4cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint4sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint4sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint82cmp(bigint, smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint82cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint84cmp(bigint, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint84cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint8cmp(bigint, bigint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint8cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btint8sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btint8sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.btnamecmp(name, name)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btnamecmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btnamesortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btnamesortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.btoidcmp(oid, oid)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btoidcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btoidsortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btoidsortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.btoidvectorcmp(oidvector, oidvector)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btoidvectorcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btrecordcmp(record, record)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btrecordcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btrecordimagecmp(record, record)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btrecordimagecmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btreltimecmp(reltime, reltime)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btreltimecmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.btrim(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btrim1$function$

CREATE OR REPLACE FUNCTION pg_catalog.btrim(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$btrim$function$

CREATE OR REPLACE FUNCTION pg_catalog.btrim(bytea, bytea)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteatrim$function$

CREATE OR REPLACE FUNCTION pg_catalog.bttext_pattern_cmp(text, text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bttext_pattern_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.bttext_pattern_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bttext_pattern_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.bttextcmp(text, text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bttextcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.bttextsortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bttextsortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.bttidcmp(tid, tid)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bttidcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.bttintervalcmp(tinterval, tinterval)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bttintervalcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.bytea_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytea_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.bytea_string_agg_finalfn(internal)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$bytea_string_agg_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.bytea_string_agg_transfn(internal, bytea, bytea)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$bytea_string_agg_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteacat(bytea, bytea)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteacat$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteacmp(bytea, bytea)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteacmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteaeq(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$byteaeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteage(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$byteage$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteagt(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$byteagt$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteain(cstring)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteain$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteale(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$byteale$function$

CREATE OR REPLACE FUNCTION pg_catalog.bytealike(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytealike$function$

CREATE OR REPLACE FUNCTION pg_catalog.bytealt(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bytealt$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteane(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$byteane$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteanlike(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteanlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteaout(bytea)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaout$function$

CREATE OR REPLACE FUNCTION pg_catalog.bytearecv(internal)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytearecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.byteasend(bytea)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteasend$function$

CREATE OR REPLACE FUNCTION pg_catalog.cardinality(anyarray)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$array_cardinality$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_cmp(money, money)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_div_cash(money, money)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_div_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_div_flt4(money, real)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_div_flt4$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_div_flt8(money, double precision)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_div_flt8$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_div_int2(money, smallint)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_div_int2$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_div_int4(money, integer)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_div_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_eq(money, money)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$cash_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_ge(money, money)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$cash_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_gt(money, money)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$cash_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_in(cstring)
 RETURNS money
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$cash_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_le(money, money)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$cash_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_lt(money, money)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$cash_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_mi(money, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_mul_flt4(money, real)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_mul_flt4$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_mul_flt8(money, double precision)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_mul_flt8$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_mul_int2(money, smallint)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_mul_int2$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_mul_int4(money, integer)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_mul_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_ne(money, money)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$cash_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_out(money)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$cash_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_pl(money, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_recv(internal)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_send(money)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.cash_words(money)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cash_words$function$

CREATE OR REPLACE FUNCTION pg_catalog.cashlarger(money, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cashlarger$function$

CREATE OR REPLACE FUNCTION pg_catalog.cashsmaller(money, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cashsmaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.cbrt(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dcbrt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ceil(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dceil$function$

CREATE OR REPLACE FUNCTION pg_catalog.ceil(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_ceil$function$

CREATE OR REPLACE FUNCTION pg_catalog.ceiling(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dceil$function$

CREATE OR REPLACE FUNCTION pg_catalog.ceiling(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_ceil$function$

CREATE OR REPLACE FUNCTION pg_catalog.center(circle)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.center(box)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_center$function$

CREATE OR REPLACE FUNCTION pg_catalog."char"(text)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_char$function$

CREATE OR REPLACE FUNCTION pg_catalog."char"(integer)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i4tochar$function$

CREATE OR REPLACE FUNCTION pg_catalog.char_length(text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.char_length(character)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.character_length(text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.character_length(character)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.chareq("char", "char")
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$chareq$function$

CREATE OR REPLACE FUNCTION pg_catalog.charge("char", "char")
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$charge$function$

CREATE OR REPLACE FUNCTION pg_catalog.chargt("char", "char")
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$chargt$function$

CREATE OR REPLACE FUNCTION pg_catalog.charin(cstring)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$charin$function$

CREATE OR REPLACE FUNCTION pg_catalog.charle("char", "char")
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$charle$function$

CREATE OR REPLACE FUNCTION pg_catalog.charlt("char", "char")
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$charlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.charne("char", "char")
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$charne$function$

CREATE OR REPLACE FUNCTION pg_catalog.charout("char")
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$charout$function$

CREATE OR REPLACE FUNCTION pg_catalog.charrecv(internal)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$charrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.charsend("char")
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$charsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.chr(integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$chr$function$

CREATE OR REPLACE FUNCTION pg_catalog.cideq(cid, cid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$cideq$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidin(cstring)
 RETURNS cid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidin$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidout(cid)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidout$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidr(inet)
 RETURNS cidr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_to_cidr$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidr_in(cstring)
 RETURNS cidr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidr_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidr_out(cidr)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidr_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidr_recv(internal)
 RETURNS cidr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidr_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidr_send(cidr)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidr_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidrecv(internal)
 RETURNS cid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.cidsend(cid)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle(box)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_circle$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle(polygon)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_circle$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle(point, double precision)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cr_circle$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_above(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_above$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_add_pt(circle, point)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_add_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_below(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_below$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_center(circle)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_contain(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_contain$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_contain_pt(circle, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_contain_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_contained(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_contained$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_distance(circle, circle)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_div_pt(circle, point)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_div_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_eq(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$circle_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_ge(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$circle_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_gt(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$circle_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_in(cstring)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_le(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$circle_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_left(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_left$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_lt(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$circle_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_mul_pt(circle, point)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_mul_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_ne(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$circle_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_out(circle)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_overabove(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_overabove$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_overbelow(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_overbelow$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_overlap(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_overlap$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_overleft(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_overleft$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_overright(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_overright$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_recv(internal)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_right(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_right$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_same(circle, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_send(circle)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.circle_sub_pt(circle, point)
 RETURNS circle
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_sub_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.clock_timestamp()
 RETURNS timestamp with time zone
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$clock_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_lb(line, box)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_lb$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_ls(line, lseg)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_ls$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_lseg(lseg, lseg)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_lseg$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_pb(point, box)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_pb$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_pl(point, line)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_ps(point, lseg)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_ps$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_sb(lseg, box)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_sb$function$

CREATE OR REPLACE FUNCTION pg_catalog.close_sl(lseg, line)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$close_sl$function$

CREATE OR REPLACE FUNCTION pg_catalog.col_description(oid, integer)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT
AS $function$select description from pg_catalog.pg_description where objoid = $1 and classoid = 'pg_catalog.pg_class'::pg_catalog.regclass and objsubid = $2$function$

CREATE OR REPLACE FUNCTION pg_catalog.concat(VARIADIC "any")
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$text_concat$function$

CREATE OR REPLACE FUNCTION pg_catalog.concat_ws(text, VARIADIC "any")
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$text_concat_ws$function$

CREATE OR REPLACE FUNCTION pg_catalog.contjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$contjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.contsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$contsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.convert(bytea, name, name)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_convert$function$

CREATE OR REPLACE FUNCTION pg_catalog.convert_from(bytea, name)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_convert_from$function$

CREATE OR REPLACE FUNCTION pg_catalog.convert_to(text, name)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_convert_to$function$

CREATE OR REPLACE FUNCTION pg_catalog.cos(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dcos$function$

CREATE OR REPLACE FUNCTION pg_catalog.cosd(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dcosd$function$

CREATE OR REPLACE FUNCTION pg_catalog.cot(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dcot$function$

CREATE OR REPLACE FUNCTION pg_catalog.cotd(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dcotd$function$

CREATE OR REPLACE FUNCTION pg_catalog.cstring_in(cstring)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cstring_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.cstring_out(cstring)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cstring_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.cstring_recv(internal)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$cstring_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.cstring_send(cstring)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$cstring_send$function$

create function cume_dist window_cume_dist

CREATE OR REPLACE FUNCTION pg_catalog.cume_dist()
 RETURNS double precision
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE
AS $function$window_cume_dist$function$

CREATE OR REPLACE FUNCTION pg_catalog.cume_dist_final(internal, VARIADIC "any")
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$hypothetical_cume_dist_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.current_database()
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$current_database$function$

CREATE OR REPLACE FUNCTION pg_catalog.current_query()
 RETURNS text
 LANGUAGE internal
 PARALLEL RESTRICTED
AS $function$current_query$function$

CREATE OR REPLACE FUNCTION pg_catalog."current_schema"()
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$current_schema$function$

CREATE OR REPLACE FUNCTION pg_catalog.current_schemas(boolean)
 RETURNS name[]
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$current_schemas$function$

CREATE OR REPLACE FUNCTION pg_catalog.current_setting(text, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$show_config_by_name_missing_ok$function$

CREATE OR REPLACE FUNCTION pg_catalog.current_setting(text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$show_config_by_name$function$

CREATE OR REPLACE FUNCTION pg_catalog."current_user"()
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$current_user$function$

CREATE OR REPLACE FUNCTION pg_catalog.currtid(oid, tid)
 RETURNS tid
 LANGUAGE internal
 STRICT
AS $function$currtid_byreloid$function$

CREATE OR REPLACE FUNCTION pg_catalog.currtid2(text, tid)
 RETURNS tid
 LANGUAGE internal
 STRICT
AS $function$currtid_byrelname$function$

CREATE OR REPLACE FUNCTION pg_catalog.currval(regclass)
 RETURNS bigint
 LANGUAGE internal
 STRICT
AS $function$currval_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.cursor_to_xml(cursor refcursor, count integer, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$cursor_to_xml$function$

CREATE OR REPLACE FUNCTION pg_catalog.cursor_to_xmlschema(cursor refcursor, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$cursor_to_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.database_to_xml(nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$database_to_xml$function$

CREATE OR REPLACE FUNCTION pg_catalog.database_to_xml_and_xmlschema(nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$database_to_xml_and_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.database_to_xmlschema(nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$database_to_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.date(timestamp without time zone)
 RETURNS date
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.date(abstime)
 RETURNS date
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$abstime_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.date(timestamp with time zone)
 RETURNS date
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_cmp(date, date)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_cmp_timestamp(date, timestamp without time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_cmp_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_cmp_timestamptz(date, timestamp with time zone)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_cmp_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_eq(date, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$date_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_eq_timestamp(date, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_eq_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_eq_timestamptz(date, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_eq_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_ge(date, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$date_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_ge_timestamp(date, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_ge_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_ge_timestamptz(date, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_ge_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_gt(date, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$date_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_gt_timestamp(date, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_gt_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_gt_timestamptz(date, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_gt_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_in(cstring)
 RETURNS date
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_larger(date, date)
 RETURNS date
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_le(date, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$date_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_le_timestamp(date, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_le_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_le_timestamptz(date, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_le_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_lt(date, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$date_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_lt_timestamp(date, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_lt_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_lt_timestamptz(date, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_lt_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_mi(date, date)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_mi_interval(date, interval)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_mi_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_mii(date, integer)
 RETURNS date
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_mii$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_ne(date, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$date_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_ne_timestamp(date, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_ne_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_ne_timestamptz(date, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_ne_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_out(date)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, timestamp without time zone)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_part$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, time without time zone)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_part$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, date)
 RETURNS double precision
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.date_part($1, cast($2 as timestamp without time zone))$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, reltime)
 RETURNS double precision
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.date_part($1, cast($2 as pg_catalog.interval))$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, abstime)
 RETURNS double precision
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.date_part($1, cast($2 as timestamp with time zone))$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, time with time zone)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_part$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, interval)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_part$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_part(text, timestamp with time zone)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_part$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_pl_interval(date, interval)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_pl_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_pli(date, integer)
 RETURNS date
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_pli$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_recv(internal)
 RETURNS date
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_send(date)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_smaller(date, date)
 RETURNS date
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_trunc(text, timestamp without time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_trunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_trunc(text, interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_trunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.date_trunc(text, timestamp with time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_trunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.daterange(date, date, text)
 RETURNS daterange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor3$function$

CREATE OR REPLACE FUNCTION pg_catalog.daterange(date, date)
 RETURNS daterange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor2$function$

CREATE OR REPLACE FUNCTION pg_catalog.daterange_canonical(daterange)
 RETURNS daterange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$daterange_canonical$function$

CREATE OR REPLACE FUNCTION pg_catalog.daterange_subdiff(date, date)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$daterange_subdiff$function$

CREATE OR REPLACE FUNCTION pg_catalog.datetime_pl(date, time without time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datetime_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.datetimetz_pl(date, time with time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datetimetz_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.dcbrt(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dcbrt$function$

CREATE OR REPLACE FUNCTION pg_catalog.decode(text, text)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$binary_decode$function$

CREATE OR REPLACE FUNCTION pg_catalog.degrees(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$degrees$function$

create function dense_rank window_dense_rank

CREATE OR REPLACE FUNCTION pg_catalog.dense_rank()
 RETURNS bigint
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE
AS $function$window_dense_rank$function$

CREATE OR REPLACE FUNCTION pg_catalog.dense_rank_final(internal, VARIADIC "any")
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$hypothetical_dense_rank_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.dexp(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dexp$function$

CREATE OR REPLACE FUNCTION pg_catalog.diagonal(box)
 RETURNS lseg
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_diagonal$function$

CREATE OR REPLACE FUNCTION pg_catalog.diameter(circle)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_diameter$function$

CREATE OR REPLACE FUNCTION pg_catalog.dispell_init(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dispell_init$function$

CREATE OR REPLACE FUNCTION pg_catalog.dispell_lexize(internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dispell_lexize$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_cpoint(circle, point)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_cpoint$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_cpoly(circle, polygon)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_cpoly$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_lb(line, box)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_lb$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_pb(point, box)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_pb$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_pc(point, circle)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_pc$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_pl(point, line)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_polyp(polygon, point)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_polyp$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_ppath(point, path)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_ppath$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_ppoly(point, polygon)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_ppoly$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_ps(point, lseg)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_ps$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_sb(lseg, box)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_sb$function$

CREATE OR REPLACE FUNCTION pg_catalog.dist_sl(lseg, line)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dist_sl$function$

CREATE OR REPLACE FUNCTION pg_catalog.div(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_div_trunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.dlog1(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dlog1$function$

CREATE OR REPLACE FUNCTION pg_catalog.dlog10(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dlog10$function$

CREATE OR REPLACE FUNCTION pg_catalog.domain_in(cstring, oid, integer)
 RETURNS "any"
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$domain_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.domain_recv(internal, oid, integer)
 RETURNS "any"
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$domain_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.dpow(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dpow$function$

CREATE OR REPLACE FUNCTION pg_catalog.dround(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dround$function$

CREATE OR REPLACE FUNCTION pg_catalog.dsimple_init(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsimple_init$function$

CREATE OR REPLACE FUNCTION pg_catalog.dsimple_lexize(internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsimple_lexize$function$

CREATE OR REPLACE FUNCTION pg_catalog.dsnowball_init(internal)
 RETURNS internal
 LANGUAGE c
 STRICT
AS '$libdir/dict_snowball', $function$dsnowball_init$function$

CREATE OR REPLACE FUNCTION pg_catalog.dsnowball_lexize(internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE c
 STRICT
AS '$libdir/dict_snowball', $function$dsnowball_lexize$function$

CREATE OR REPLACE FUNCTION pg_catalog.dsqrt(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsqrt$function$

CREATE OR REPLACE FUNCTION pg_catalog.dsynonym_init(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsynonym_init$function$

CREATE OR REPLACE FUNCTION pg_catalog.dsynonym_lexize(internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsynonym_lexize$function$

CREATE OR REPLACE FUNCTION pg_catalog.dtrunc(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtrunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.elem_contained_by_range(anyelement, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$elem_contained_by_range$function$

CREATE OR REPLACE FUNCTION pg_catalog.encode(bytea, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$binary_encode$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_cmp(anyenum, anyenum)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_eq(anyenum, anyenum)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_first(anyenum)
 RETURNS anyenum
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$enum_first$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_ge(anyenum, anyenum)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_gt(anyenum, anyenum)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_in(cstring, oid)
 RETURNS anyenum
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$enum_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_larger(anyenum, anyenum)
 RETURNS anyenum
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_last(anyenum)
 RETURNS anyenum
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$enum_last$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_le(anyenum, anyenum)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_lt(anyenum, anyenum)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_ne(anyenum, anyenum)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_out(anyenum)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$enum_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_range(anyenum)
 RETURNS anyarray
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$enum_range_all$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_range(anyenum, anyenum)
 RETURNS anyarray
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$enum_range_bounds$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_recv(internal, oid)
 RETURNS anyenum
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$enum_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_send(anyenum)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$enum_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.enum_smaller(anyenum, anyenum)
 RETURNS anyenum
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$enum_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.eqjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$eqjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.eqsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$eqsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_cn_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_cn_and_mic', $function$euc_cn_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_cn_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_cn', $function$euc_cn_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_jis_2004_to_shift_jis_2004(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc2004_sjis2004', $function$euc_jis_2004_to_shift_jis_2004$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_jis_2004_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc2004', $function$euc_jis_2004_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_jp_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_jp_and_sjis', $function$euc_jp_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_jp_to_sjis(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_jp_and_sjis', $function$euc_jp_to_sjis$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_jp_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_jp', $function$euc_jp_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_kr_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_kr_and_mic', $function$euc_kr_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_kr_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_kr', $function$euc_kr_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_tw_to_big5(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_tw_and_big5', $function$euc_tw_to_big5$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_tw_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_tw_and_big5', $function$euc_tw_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.euc_tw_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_tw', $function$euc_tw_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.event_trigger_in(cstring)
 RETURNS event_trigger
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$event_trigger_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.event_trigger_out(event_trigger)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$event_trigger_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.exp(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_exp$function$

CREATE OR REPLACE FUNCTION pg_catalog.exp(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dexp$function$

CREATE OR REPLACE FUNCTION pg_catalog.factorial(bigint)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_fac$function$

CREATE OR REPLACE FUNCTION pg_catalog.family(inet)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_family$function$

CREATE OR REPLACE FUNCTION pg_catalog.fdw_handler_in(cstring)
 RETURNS fdw_handler
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$fdw_handler_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.fdw_handler_out(fdw_handler)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$fdw_handler_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.first_value(anyelement)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_first_value$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4(bigint)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i8tof$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4(integer)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i4tof$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4(double precision)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtof$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4(smallint)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i2tof$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4(numeric)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_float4$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48div(real, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float48div$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48eq(real, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float48eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48ge(real, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float48ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48gt(real, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float48gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48le(real, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float48le$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48lt(real, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float48lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48mi(real, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float48mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48mul(real, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float48mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48ne(real, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float48ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.float48pl(real, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float48pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4_accum(double precision[], real)
 RETURNS double precision[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4abs(real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4div(real, real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4div$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4eq(real, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float4eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4ge(real, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float4ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4gt(real, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float4gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4in(cstring)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4in$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4larger(real, real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4le(real, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float4le$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4lt(real, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float4lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4mi(real, real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4mul(real, real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4ne(real, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float4ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4out(real)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4out$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4pl(real, real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4recv(internal)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4send(real)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4send$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4smaller(real, real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4um(real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4um$function$

CREATE OR REPLACE FUNCTION pg_catalog.float4up(real)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4up$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8(bigint)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i8tod$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8(integer)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i4tod$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8(real)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ftod$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8(smallint)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i2tod$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8(numeric)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_float8$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84div(double precision, real)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float84div$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84eq(double precision, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float84eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84ge(double precision, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float84ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84gt(double precision, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float84gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84le(double precision, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float84le$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84lt(double precision, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float84lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84mi(double precision, real)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float84mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84mul(double precision, real)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float84mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84ne(double precision, real)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float84ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.float84pl(double precision, real)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float84pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_accum(double precision[], double precision)
 RETURNS double precision[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_avg(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_avg$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_combine(double precision[], double precision[])
 RETURNS double precision[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_corr(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_corr$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_covar_pop(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_covar_pop$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_covar_samp(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_covar_samp$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_accum(double precision[], double precision, double precision)
 RETURNS double precision[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_avgx(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_avgx$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_avgy(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_avgy$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_combine(double precision[], double precision[])
 RETURNS double precision[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_intercept(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_intercept$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_r2(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_r2$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_slope(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_slope$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_sxx(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_sxx$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_sxy(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_sxy$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_regr_syy(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_regr_syy$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_stddev_pop(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_stddev_pop$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_stddev_samp(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_stddev_samp$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_var_pop(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_var_pop$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8_var_samp(double precision[])
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_var_samp$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8abs(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8div(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8div$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8eq(double precision, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float8eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8ge(double precision, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float8ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8gt(double precision, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float8gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8in(cstring)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8in$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8larger(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8le(double precision, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float8le$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8lt(double precision, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float8lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8mi(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8mul(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8ne(double precision, double precision)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$float8ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8out(double precision)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8out$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8pl(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8recv(internal)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8send(double precision)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8send$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8smaller(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8um(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8um$function$

CREATE OR REPLACE FUNCTION pg_catalog.float8up(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8up$function$

CREATE OR REPLACE FUNCTION pg_catalog.floor(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dfloor$function$

CREATE OR REPLACE FUNCTION pg_catalog.floor(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_floor$function$

CREATE OR REPLACE FUNCTION pg_catalog.flt4_mul_cash(real, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$flt4_mul_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.flt8_mul_cash(double precision, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$flt8_mul_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.fmgr_c_validator(oid)
 RETURNS void
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$fmgr_c_validator$function$

CREATE OR REPLACE FUNCTION pg_catalog.fmgr_internal_validator(oid)
 RETURNS void
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$fmgr_internal_validator$function$

CREATE OR REPLACE FUNCTION pg_catalog.fmgr_sql_validator(oid)
 RETURNS void
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$fmgr_sql_validator$function$

CREATE OR REPLACE FUNCTION pg_catalog.format(text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$text_format_nv$function$

CREATE OR REPLACE FUNCTION pg_catalog.format(text, VARIADIC "any")
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$text_format$function$

CREATE OR REPLACE FUNCTION pg_catalog.format_type(oid, integer)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$format_type$function$

CREATE OR REPLACE FUNCTION pg_catalog.gb18030_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_gb18030', $function$gb18030_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.gbk_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_gbk', $function$gbk_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(timestamp with time zone, timestamp with time zone, interval)
 RETURNS SETOF timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$generate_series_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(timestamp without time zone, timestamp without time zone, interval)
 RETURNS SETOF timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_series_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(numeric, numeric)
 RETURNS SETOF numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_series_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(numeric, numeric, numeric)
 RETURNS SETOF numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_series_step_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(bigint, bigint)
 RETURNS SETOF bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_series_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(bigint, bigint, bigint)
 RETURNS SETOF bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_series_step_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(integer, integer)
 RETURNS SETOF integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_series_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_series(integer, integer, integer)
 RETURNS SETOF integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_series_step_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_subscripts(anyarray, integer)
 RETURNS SETOF integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_subscripts_nodir$function$

CREATE OR REPLACE FUNCTION pg_catalog.generate_subscripts(anyarray, integer, boolean)
 RETURNS SETOF integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$generate_subscripts$function$

CREATE OR REPLACE FUNCTION pg_catalog.get_bit(bytea, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaGetBit$function$

CREATE OR REPLACE FUNCTION pg_catalog.get_bit(bit, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitgetbit$function$

CREATE OR REPLACE FUNCTION pg_catalog.get_byte(bytea, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaGetByte$function$

CREATE OR REPLACE FUNCTION pg_catalog.get_current_ts_config()
 RETURNS regconfig
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$get_current_ts_config$function$

CREATE OR REPLACE FUNCTION pg_catalog.getdatabaseencoding()
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$getdatabaseencoding$function$

CREATE OR REPLACE FUNCTION pg_catalog.getpgusername()
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$current_user$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_clean_pending_list(regclass)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$gin_clean_pending_list$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_cmp_prefix(text, text, smallint, internal)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_cmp_prefix$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_cmp_tslexeme(text, text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_cmp_tslexeme$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_compare_jsonb(text, text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_compare_jsonb$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_consistent_jsonb(internal, smallint, jsonb, integer, internal, internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_consistent_jsonb$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_consistent_jsonb_path(internal, smallint, jsonb, integer, internal, internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_consistent_jsonb_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_jsonb(jsonb, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_jsonb$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_jsonb_path(jsonb, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_jsonb_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_jsonb_query(jsonb, internal, smallint, internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_jsonb_query$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_jsonb_query_path(jsonb, internal, smallint, internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_jsonb_query_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_tsquery(tsquery, internal, smallint, internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_tsquery_oldsig$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_tsquery(tsvector, internal, smallint, internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_tsquery$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_tsquery(tsquery, internal, smallint, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_tsquery_5args$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_tsvector(tsvector, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_tsvector$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_extract_tsvector(tsvector, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_extract_tsvector_2args$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_triconsistent_jsonb(internal, smallint, jsonb, integer, internal, internal, internal)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_triconsistent_jsonb$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_triconsistent_jsonb_path(internal, smallint, jsonb, integer, internal, internal, internal)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_triconsistent_jsonb_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_tsquery_consistent(internal, smallint, tsquery, integer, internal, internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_tsquery_consistent_oldsig$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_tsquery_consistent(internal, smallint, tsvector, integer, internal, internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_tsquery_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_tsquery_consistent(internal, smallint, tsquery, integer, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_tsquery_consistent_6args$function$

CREATE OR REPLACE FUNCTION pg_catalog.gin_tsquery_triconsistent(internal, smallint, tsvector, integer, internal, internal, internal)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gin_tsquery_triconsistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.ginarrayconsistent(internal, smallint, anyarray, integer, internal, internal, internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ginarrayconsistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.ginarrayextract(anyarray, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ginarrayextract_2args$function$

CREATE OR REPLACE FUNCTION pg_catalog.ginarrayextract(anyarray, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ginarrayextract$function$

CREATE OR REPLACE FUNCTION pg_catalog.ginarraytriconsistent(internal, smallint, anyarray, integer, internal, internal, internal)
 RETURNS "char"
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ginarraytriconsistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.ginhandler(internal)
 RETURNS index_am_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$ginhandler$function$

CREATE OR REPLACE FUNCTION pg_catalog.ginqueryarrayextract(anyarray, internal, smallint, internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ginqueryarrayextract$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_consistent(internal, box, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_decompress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_decompress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_fetch(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_fetch$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_penalty(internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_penalty$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_picksplit(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_same(box, box, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_box_union(internal, internal)
 RETURNS box
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_box_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_circle_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_circle_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_circle_consistent(internal, circle, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_circle_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_circle_distance(internal, circle, smallint, oid, internal)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_circle_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_point_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_point_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_point_consistent(internal, point, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_point_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_point_distance(internal, point, smallint, oid, internal)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_point_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_point_fetch(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_point_fetch$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_poly_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_poly_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_poly_consistent(internal, polygon, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_poly_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.gist_poly_distance(internal, polygon, smallint, oid, internal)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gist_poly_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.gisthandler(internal)
 RETURNS index_am_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$gisthandler$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_consistent(internal, internal, integer, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_consistent_oldsig$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_consistent(internal, tsquery, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_decompress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_decompress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_penalty(internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_penalty$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_picksplit(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_same(bigint, bigint, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsquery_union(internal, internal)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsquery_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_consistent(internal, gtsvector, integer, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_consistent_oldsig$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_consistent(internal, tsvector, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_decompress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_decompress$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_penalty(internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_penalty$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_picksplit(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_same(gtsvector, gtsvector, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvector_union(internal, internal)
 RETURNS gtsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvector_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvectorin(cstring)
 RETURNS gtsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvectorin$function$

CREATE OR REPLACE FUNCTION pg_catalog.gtsvectorout(gtsvector)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$gtsvectorout$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_any_column_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$has_any_column_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_any_column_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$has_any_column_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_any_column_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$has_any_column_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_any_column_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$has_any_column_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_any_column_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$has_any_column_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_any_column_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$has_any_column_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(oid, smallint, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_id_attnum$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(text, smallint, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_name_attnum$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(text, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(oid, oid, smallint, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_id_id_attnum$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(oid, oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_id_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(oid, text, smallint, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_id_name_attnum$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(oid, text, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_id_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(name, oid, smallint, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_name_id_attnum$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(name, oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_name_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(name, text, smallint, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_name_name_attnum$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_column_privilege(name, text, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_column_privilege_name_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_database_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_database_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_database_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_database_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_database_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_database_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_database_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_database_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_database_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_database_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_database_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_database_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_foreign_data_wrapper_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_foreign_data_wrapper_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_foreign_data_wrapper_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_foreign_data_wrapper_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_foreign_data_wrapper_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_foreign_data_wrapper_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_foreign_data_wrapper_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_foreign_data_wrapper_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_foreign_data_wrapper_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_foreign_data_wrapper_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_foreign_data_wrapper_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_foreign_data_wrapper_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_function_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_function_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_function_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_function_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_function_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_function_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_function_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_function_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_function_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_function_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_function_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_function_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_language_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_language_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_language_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_language_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_language_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_language_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_language_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_language_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_language_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_language_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_language_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_language_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_schema_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_schema_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_schema_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_schema_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_schema_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_schema_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_schema_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_schema_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_schema_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_schema_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_schema_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_schema_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_sequence_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_sequence_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_sequence_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_sequence_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_sequence_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_sequence_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_sequence_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_sequence_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_sequence_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_sequence_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_sequence_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_sequence_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_server_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_server_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_server_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_server_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_server_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_server_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_server_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_server_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_server_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_server_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_server_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_server_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_table_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_table_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_table_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_table_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_table_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_table_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_table_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_table_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_table_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_table_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_table_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_table_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_tablespace_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_tablespace_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_tablespace_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_tablespace_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_tablespace_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_tablespace_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_tablespace_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_tablespace_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_tablespace_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_tablespace_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_tablespace_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_tablespace_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_type_privilege(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_type_privilege_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_type_privilege(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_type_privilege_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_type_privilege(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_type_privilege_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_type_privilege(oid, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_type_privilege_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_type_privilege(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_type_privilege_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.has_type_privilege(name, text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$has_type_privilege_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.hash_aclitem(aclitem)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hash_aclitem$function$

CREATE OR REPLACE FUNCTION pg_catalog.hash_array(anyarray)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hash_array$function$

CREATE OR REPLACE FUNCTION pg_catalog.hash_numeric(numeric)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hash_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog.hash_range(anyrange)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hash_range$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashbpchar(character)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashbpchar$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashchar("char")
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashchar$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashenum(anyenum)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashenum$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashfloat4(real)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashfloat4$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashfloat8(double precision)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashfloat8$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashhandler(internal)
 RETURNS index_am_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$hashhandler$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashinet(inet)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashinet$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashint2(smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashint2$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashint2vector(int2vector)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashint2vector$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashint4(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashint4$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashint8(bigint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashint8$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashmacaddr(macaddr)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashmacaddr$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashname(name)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashname$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashoid(oid)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashoid$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashoidvector(oidvector)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashoidvector$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashtext(text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashtext$function$

CREATE OR REPLACE FUNCTION pg_catalog.hashvarlena(internal)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$hashvarlena$function$

CREATE OR REPLACE FUNCTION pg_catalog.height(box)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_height$function$

CREATE OR REPLACE FUNCTION pg_catalog.host(inet)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_host$function$

CREATE OR REPLACE FUNCTION pg_catalog.hostmask(inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_hostmask$function$

CREATE OR REPLACE FUNCTION pg_catalog.iclikejoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$iclikejoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.iclikesel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$iclikesel$function$

CREATE OR REPLACE FUNCTION pg_catalog.icnlikejoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$icnlikejoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.icnlikesel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$icnlikesel$function$

CREATE OR REPLACE FUNCTION pg_catalog.icregexeqjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$icregexeqjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.icregexeqsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$icregexeqsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.icregexnejoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$icregexnejoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.icregexnesel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$icregexnesel$function$

CREATE OR REPLACE FUNCTION pg_catalog.index_am_handler_in(cstring)
 RETURNS index_am_handler
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$index_am_handler_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.index_am_handler_out(index_am_handler)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$index_am_handler_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_client_addr()
 RETURNS inet
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED
AS $function$inet_client_addr$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_client_port()
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED
AS $function$inet_client_port$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_consistent(internal, inet, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_decompress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_decompress$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_fetch(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_fetch$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_penalty(internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_penalty$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_picksplit(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_same(inet, inet, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_gist_union(internal, internal)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_gist_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_in(cstring)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_merge(inet, inet)
 RETURNS cidr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_merge$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_out(inet)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_recv(internal)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_same_family(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_same_family$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_send(inet)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_server_addr()
 RETURNS inet
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$inet_server_addr$function$

CREATE OR REPLACE FUNCTION pg_catalog.inet_server_port()
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$inet_server_port$function$

CREATE OR REPLACE FUNCTION pg_catalog.inetand(inet, inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inetand$function$

CREATE OR REPLACE FUNCTION pg_catalog.inetmi(inet, inet)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inetmi$function$

CREATE OR REPLACE FUNCTION pg_catalog.inetmi_int8(inet, bigint)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inetmi_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.inetnot(inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inetnot$function$

CREATE OR REPLACE FUNCTION pg_catalog.inetor(inet, inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inetor$function$

CREATE OR REPLACE FUNCTION pg_catalog.inetpl(inet, bigint)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inetpl$function$

CREATE OR REPLACE FUNCTION pg_catalog.initcap(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$initcap$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2(bigint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int82$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2(integer)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i4toi2$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2(real)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ftoi2$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2(double precision)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtoi2$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2(numeric)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_int2$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24div(smallint, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int24div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24eq(smallint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int24eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24ge(smallint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int24ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24gt(smallint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int24gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24le(smallint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int24le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24lt(smallint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int24lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24mi(smallint, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int24mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24mul(smallint, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int24mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24ne(smallint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int24ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int24pl(smallint, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int24pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28div(smallint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int28div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28eq(smallint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int28eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28ge(smallint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int28ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28gt(smallint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int28gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28le(smallint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int28le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28lt(smallint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int28lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28mi(smallint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int28mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28mul(smallint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int28mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28ne(smallint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int28ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int28pl(smallint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int28pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2_accum(internal, smallint)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int2_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2_accum_inv(internal, smallint)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int2_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2_avg_accum(bigint[], smallint)
 RETURNS bigint[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2_avg_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2_avg_accum_inv(bigint[], smallint)
 RETURNS bigint[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2_avg_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2_mul_cash(smallint, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2_mul_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2_sum(bigint, smallint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int2_sum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2abs(smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2and(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2and$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2div(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2eq(smallint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int2eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2ge(smallint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int2ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2gt(smallint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int2gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2in(cstring)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2in$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2int4_sum(bigint[])
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2int4_sum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2larger(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2le(smallint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int2le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2lt(smallint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int2lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2mi(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2mod(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2mul(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2ne(smallint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int2ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2not(smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2not$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2or(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2or$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2out(smallint)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2out$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2pl(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2recv(internal)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2send(smallint)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2send$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2shl(smallint, integer)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2shl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2shr(smallint, integer)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2shr$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2smaller(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2um(smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2um$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2up(smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2up$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2vectoreq(int2vector, int2vector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2vectoreq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2vectorin(cstring)
 RETURNS int2vector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2vectorin$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2vectorout(int2vector)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2vectorout$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2vectorrecv(internal)
 RETURNS int2vector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2vectorrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2vectorsend(int2vector)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2vectorsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.int2xor(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2xor$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4("char")
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$chartoi4$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4(bigint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int84$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4(real)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ftoi4$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4(double precision)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtoi4$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4(smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i2toi4$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4(boolean)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bool_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4(numeric)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4(bit)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bittoint4$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42div(integer, smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int42div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42eq(integer, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int42eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42ge(integer, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int42ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42gt(integer, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int42gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42le(integer, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int42le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42lt(integer, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int42lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42mi(integer, smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int42mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42mul(integer, smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int42mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42ne(integer, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int42ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int42pl(integer, smallint)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int42pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48div(integer, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int48div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48eq(integer, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int48eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48ge(integer, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int48ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48gt(integer, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int48gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48le(integer, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int48le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48lt(integer, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int48lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48mi(integer, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int48mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48mul(integer, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int48mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48ne(integer, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int48ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int48pl(integer, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int48pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4_accum(internal, integer)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int4_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4_accum_inv(internal, integer)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int4_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4_avg_accum(bigint[], integer)
 RETURNS bigint[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4_avg_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4_avg_accum_inv(bigint[], integer)
 RETURNS bigint[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4_avg_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4_avg_combine(bigint[], bigint[])
 RETURNS bigint[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4_avg_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4_mul_cash(integer, money)
 RETURNS money
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4_mul_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4_sum(bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int4_sum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4abs(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4and(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4and$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4div(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4eq(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int4eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4ge(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int4ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4gt(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int4gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4in(cstring)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4in$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4inc(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4inc$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4larger(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4le(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int4le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4lt(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int4lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4mi(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4mod(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4mul(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4ne(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int4ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4not(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4not$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4or(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4or$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4out(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4out$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4pl(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4range(integer, integer, text)
 RETURNS int4range
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor3$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4range(integer, integer)
 RETURNS int4range
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor2$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4range_canonical(int4range)
 RETURNS int4range
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4range_canonical$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4range_subdiff(integer, integer)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4range_subdiff$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4recv(internal)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4send(integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4send$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4shl(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4shl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4shr(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4shr$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4smaller(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4um(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4um$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4up(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4up$function$

CREATE OR REPLACE FUNCTION pg_catalog.int4xor(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4xor$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8(smallint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int28$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8(real)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ftoi8$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8(double precision)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtoi8$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8(integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int48$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8(bit)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bittoint8$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8(numeric)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8(oid)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidtoi8$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82div(bigint, smallint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int82div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82eq(bigint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int82eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82ge(bigint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int82ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82gt(bigint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int82gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82le(bigint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int82le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82lt(bigint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int82lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82mi(bigint, smallint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int82mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82mul(bigint, smallint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int82mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82ne(bigint, smallint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int82ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int82pl(bigint, smallint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int82pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84div(bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int84div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84eq(bigint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int84eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84ge(bigint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int84ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84gt(bigint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int84gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84le(bigint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int84le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84lt(bigint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int84lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84mi(bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int84mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84mul(bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int84mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84ne(bigint, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int84ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int84pl(bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int84pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_accum(internal, bigint)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int8_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_accum_inv(internal, bigint)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int8_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_avg(bigint[])
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8_avg$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_avg_accum(internal, bigint)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int8_avg_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_avg_accum_inv(internal, bigint)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int8_avg_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_avg_combine(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int8_avg_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_avg_deserialize(bytea, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8_avg_deserialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_avg_serialize(internal)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8_avg_serialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8_sum(numeric, bigint)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$int8_sum$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8abs(bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8and(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8and$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8dec(bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8dec$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8dec_any(bigint, "any")
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8dec_any$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8div(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8div$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8eq(bigint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int8eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8ge(bigint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int8ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8gt(bigint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int8gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8in(cstring)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8in$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8inc(bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8inc$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8inc_any(bigint, "any")
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8inc_any$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8inc_float8_float8(bigint, double precision, double precision)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8inc_float8_float8$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8larger(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8le(bigint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int8le$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8lt(bigint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int8lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8mi(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8mod(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8mul(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8ne(bigint, bigint)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$int8ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8not(bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8not$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8or(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8or$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8out(bigint)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8out$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8pl(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8pl_inet(bigint, inet)
 RETURNS inet
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select $2 + $1$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8range(bigint, bigint, text)
 RETURNS int8range
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor3$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8range(bigint, bigint)
 RETURNS int8range
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor2$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8range_canonical(int8range)
 RETURNS int8range
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8range_canonical$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8range_subdiff(bigint, bigint)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8range_subdiff$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8recv(internal)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8send(bigint)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8send$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8shl(bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8shl$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8shr(bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8shr$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8smaller(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8um(bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8um$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8up(bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8up$function$

CREATE OR REPLACE FUNCTION pg_catalog.int8xor(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8xor$function$

CREATE OR REPLACE FUNCTION pg_catalog.integer_pl_date(integer, date)
 RETURNS date
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select $2 + $1$function$

CREATE OR REPLACE FUNCTION pg_catalog.inter_lb(line, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inter_lb$function$

CREATE OR REPLACE FUNCTION pg_catalog.inter_sb(lseg, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inter_sb$function$

CREATE OR REPLACE FUNCTION pg_catalog.inter_sl(lseg, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inter_sl$function$

CREATE OR REPLACE FUNCTION pg_catalog.internal_in(cstring)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$internal_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.internal_out(internal)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$internal_out$function$

CREATE OR REPLACE FUNCTION pg_catalog."interval"(time without time zone)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog."interval"(interval, integer)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_scale$function$

CREATE OR REPLACE FUNCTION pg_catalog."interval"(reltime)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$reltime_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_accum(interval[], interval)
 RETURNS interval[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_accum_inv(interval[], interval)
 RETURNS interval[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_avg(interval[])
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_avg$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_cmp(interval, interval)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_combine(interval[], interval[])
 RETURNS interval[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_div(interval, double precision)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_div$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_eq(interval, interval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$interval_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_ge(interval, interval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$interval_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_gt(interval, interval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$interval_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_hash(interval)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_hash$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_in(cstring, oid, integer)
 RETURNS interval
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$interval_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_larger(interval, interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_le(interval, interval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$interval_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_lt(interval, interval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$interval_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_mi(interval, interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_mul(interval, double precision)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_ne(interval, interval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$interval_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_out(interval)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_pl(interval, interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_pl_date(interval, date)
 RETURNS timestamp without time zone
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select $2 + $1$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_pl_time(interval, time without time zone)
 RETURNS time without time zone
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select $2 + $1$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_pl_timestamp(interval, timestamp without time zone)
 RETURNS timestamp without time zone
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select $2 + $1$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_pl_timestamptz(interval, timestamp with time zone)
 RETURNS timestamp with time zone
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select $2 + $1$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_pl_timetz(interval, time with time zone)
 RETURNS time with time zone
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select $2 + $1$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_recv(internal, oid, integer)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_send(interval)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_smaller(interval, interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.interval_um(interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_um$function$

CREATE OR REPLACE FUNCTION pg_catalog.intervaltypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$intervaltypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.intervaltypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$intervaltypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.intinterval(abstime, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$intinterval$function$

CREATE OR REPLACE FUNCTION pg_catalog.isclosed(path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_isclosed$function$

CREATE OR REPLACE FUNCTION pg_catalog.isempty(anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_empty$function$

CREATE OR REPLACE FUNCTION pg_catalog.isfinite(abstime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$abstime_finite$function$

CREATE OR REPLACE FUNCTION pg_catalog.isfinite(timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_finite$function$

CREATE OR REPLACE FUNCTION pg_catalog.isfinite(interval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_finite$function$

CREATE OR REPLACE FUNCTION pg_catalog.isfinite(timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_finite$function$

CREATE OR REPLACE FUNCTION pg_catalog.isfinite(date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_finite$function$

CREATE OR REPLACE FUNCTION pg_catalog.ishorizontal(line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_horizontal$function$

CREATE OR REPLACE FUNCTION pg_catalog.ishorizontal(lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_horizontal$function$

CREATE OR REPLACE FUNCTION pg_catalog.ishorizontal(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_horiz$function$

CREATE OR REPLACE FUNCTION pg_catalog.iso8859_1_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_iso8859_1', $function$iso8859_1_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.iso8859_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_iso8859', $function$iso8859_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.iso_to_koi8r(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$iso_to_koi8r$function$

CREATE OR REPLACE FUNCTION pg_catalog.iso_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$iso_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.iso_to_win1251(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$iso_to_win1251$function$

CREATE OR REPLACE FUNCTION pg_catalog.iso_to_win866(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$iso_to_win866$function$

CREATE OR REPLACE FUNCTION pg_catalog.isopen(path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_isopen$function$

CREATE OR REPLACE FUNCTION pg_catalog.isparallel(line, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_parallel$function$

CREATE OR REPLACE FUNCTION pg_catalog.isparallel(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_parallel$function$

CREATE OR REPLACE FUNCTION pg_catalog.isperp(line, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_perp$function$

CREATE OR REPLACE FUNCTION pg_catalog.isperp(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_perp$function$

CREATE OR REPLACE FUNCTION pg_catalog.isvertical(line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_vertical$function$

CREATE OR REPLACE FUNCTION pg_catalog.isvertical(lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_vertical$function$

CREATE OR REPLACE FUNCTION pg_catalog.isvertical(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_vert$function$

CREATE OR REPLACE FUNCTION pg_catalog.johab_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_johab', $function$johab_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_agg_finalfn(internal)
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$json_agg_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_agg_transfn(internal, anyelement)
 RETURNS internal
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$json_agg_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_array_element(from_json json, element_index integer)
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_array_element$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_array_element_text(from_json json, element_index integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_array_element_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_array_elements(from_json json, OUT value json)
 RETURNS SETOF json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$json_array_elements$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_array_elements_text(from_json json, OUT value text)
 RETURNS SETOF text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$json_array_elements_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_array_length(json)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_array_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_build_array()
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$json_build_array_noargs$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_build_array(VARIADIC "any")
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$json_build_array$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_build_object()
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$json_build_object_noargs$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_build_object(VARIADIC "any")
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$json_build_object$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_each(from_json json, OUT key text, OUT value json)
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$json_each$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_each_text(from_json json, OUT key text, OUT value text)
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$json_each_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_extract_path(from_json json, VARIADIC path_elems text[])
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_extract_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_extract_path_text(from_json json, VARIADIC path_elems text[])
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_extract_path_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_in(cstring)
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_object(text[], text[])
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_object_two_arg$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_object(text[])
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_object$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_object_agg_finalfn(internal)
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$json_object_agg_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_object_agg_transfn(internal, "any", "any")
 RETURNS internal
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$json_object_agg_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_object_field(from_json json, field_name text)
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_object_field$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_object_field_text(from_json json, field_name text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_object_field_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_object_keys(json)
 RETURNS SETOF text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$json_object_keys$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_out(json)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_populate_record(base anyelement, from_json json, use_json_as_text boolean DEFAULT false)
 RETURNS anyelement
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$json_populate_record$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_populate_recordset(base anyelement, from_json json, use_json_as_text boolean DEFAULT false)
 RETURNS SETOF anyelement
 LANGUAGE internal
 STABLE PARALLEL SAFE ROWS 100
AS $function$json_populate_recordset$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_recv(internal)
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_send(json)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_strip_nulls(json)
 RETURNS json
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_strip_nulls$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_to_record(json)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$json_to_record$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_to_recordset(json)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE ROWS 100
AS $function$json_to_recordset$function$

CREATE OR REPLACE FUNCTION pg_catalog.json_typeof(json)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$json_typeof$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_agg_finalfn(internal)
 RETURNS jsonb
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_agg_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_agg_transfn(internal, anyelement)
 RETURNS internal
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_agg_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_array_element(from_json jsonb, element_index integer)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_array_element$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_array_element_text(from_json jsonb, element_index integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_array_element_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_array_elements(from_json jsonb, OUT value jsonb)
 RETURNS SETOF jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$jsonb_array_elements$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_array_elements_text(from_json jsonb, OUT value text)
 RETURNS SETOF text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$jsonb_array_elements_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_array_length(jsonb)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_array_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_build_array()
 RETURNS jsonb
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_build_array_noargs$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_build_array(VARIADIC "any")
 RETURNS jsonb
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_build_array$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_build_object()
 RETURNS jsonb
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_build_object_noargs$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_build_object(VARIADIC "any")
 RETURNS jsonb
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_build_object$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_cmp(jsonb, jsonb)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_concat(jsonb, jsonb)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_concat$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_contained(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_contained$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_contains(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_contains$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_delete(jsonb, integer)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_delete_idx$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_delete(jsonb, text)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_delete$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_delete_path(jsonb, text[])
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_delete_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_each(from_json jsonb, OUT key text, OUT value jsonb)
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$jsonb_each$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_each_text(from_json jsonb, OUT key text, OUT value text)
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$jsonb_each_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_eq(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_exists(jsonb, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_exists$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_exists_all(jsonb, text[])
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_exists_all$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_exists_any(jsonb, text[])
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_exists_any$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_extract_path(from_json jsonb, VARIADIC path_elems text[])
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_extract_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_extract_path_text(from_json jsonb, VARIADIC path_elems text[])
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_extract_path_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_ge(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_gt(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_hash(jsonb)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_hash$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_in(cstring)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_insert(jsonb_in jsonb, path text[], replacement jsonb, insert_after boolean DEFAULT false)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_insert$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_le(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_lt(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_ne(jsonb, jsonb)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_object(text[], text[])
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_object_two_arg$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_object(text[])
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_object$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_object_agg_finalfn(internal)
 RETURNS jsonb
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_object_agg_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_object_agg_transfn(internal, "any", "any")
 RETURNS internal
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_object_agg_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_object_field(from_json jsonb, field_name text)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_object_field$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_object_field_text(from_json jsonb, field_name text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_object_field_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_object_keys(jsonb)
 RETURNS SETOF text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$jsonb_object_keys$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_out(jsonb)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_populate_record(anyelement, jsonb)
 RETURNS anyelement
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$jsonb_populate_record$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_populate_recordset(anyelement, jsonb)
 RETURNS SETOF anyelement
 LANGUAGE internal
 STABLE PARALLEL SAFE ROWS 100
AS $function$jsonb_populate_recordset$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_pretty(jsonb)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_pretty$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_recv(internal)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_send(jsonb)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_set(jsonb_in jsonb, path text[], replacement jsonb, create_if_missing boolean DEFAULT true)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_set$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_strip_nulls(jsonb)
 RETURNS jsonb
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_strip_nulls$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_to_record(jsonb)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$jsonb_to_record$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_to_recordset(jsonb)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE ROWS 100
AS $function$jsonb_to_recordset$function$

CREATE OR REPLACE FUNCTION pg_catalog.jsonb_typeof(jsonb)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$jsonb_typeof$function$

CREATE OR REPLACE FUNCTION pg_catalog.justify_days(interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_justify_days$function$

CREATE OR REPLACE FUNCTION pg_catalog.justify_hours(interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_justify_hours$function$

CREATE OR REPLACE FUNCTION pg_catalog.justify_interval(interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_justify_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.koi8r_to_iso(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$koi8r_to_iso$function$

CREATE OR REPLACE FUNCTION pg_catalog.koi8r_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$koi8r_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.koi8r_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_cyrillic', $function$koi8r_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.koi8r_to_win1251(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$koi8r_to_win1251$function$

CREATE OR REPLACE FUNCTION pg_catalog.koi8r_to_win866(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$koi8r_to_win866$function$

CREATE OR REPLACE FUNCTION pg_catalog.koi8u_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_cyrillic', $function$koi8u_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.lag(anyelement, integer, anyelement)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_lag_with_offset_and_default$function$

CREATE OR REPLACE FUNCTION pg_catalog.lag(anyelement, integer)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_lag_with_offset$function$

CREATE OR REPLACE FUNCTION pg_catalog.lag(anyelement)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_lag$function$

CREATE OR REPLACE FUNCTION pg_catalog.language_handler_in(cstring)
 RETURNS language_handler
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$language_handler_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.language_handler_out(language_handler)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$language_handler_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.last_value(anyelement)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_last_value$function$

CREATE OR REPLACE FUNCTION pg_catalog.lastval()
 RETURNS bigint
 LANGUAGE internal
 STRICT
AS $function$lastval$function$

CREATE OR REPLACE FUNCTION pg_catalog.latin1_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin_and_mic', $function$latin1_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.latin2_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin2_and_win1250', $function$latin2_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.latin2_to_win1250(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin2_and_win1250', $function$latin2_to_win1250$function$

CREATE OR REPLACE FUNCTION pg_catalog.latin3_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin_and_mic', $function$latin3_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.latin4_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin_and_mic', $function$latin4_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.lead(anyelement, integer, anyelement)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_lead_with_offset_and_default$function$

CREATE OR REPLACE FUNCTION pg_catalog.lead(anyelement, integer)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_lead_with_offset$function$

CREATE OR REPLACE FUNCTION pg_catalog.lead(anyelement)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_lead$function$

CREATE OR REPLACE FUNCTION pg_catalog."left"(text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_left$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(tsvector)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(bytea)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaoctetlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(bytea, name)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$length_in_encoding$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(bit)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitlength$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(path)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(lseg)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(character)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.length(text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textlen$function$

CREATE OR REPLACE FUNCTION pg_catalog."like"(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytealike$function$

CREATE OR REPLACE FUNCTION pg_catalog."like"(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$namelike$function$

CREATE OR REPLACE FUNCTION pg_catalog."like"(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.like_escape(bytea, bytea)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$like_escape_bytea$function$

CREATE OR REPLACE FUNCTION pg_catalog.like_escape(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$like_escape$function$

CREATE OR REPLACE FUNCTION pg_catalog.likejoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$likejoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.likesel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$likesel$function$

CREATE OR REPLACE FUNCTION pg_catalog.line(point, point)
 RETURNS line
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_construct_pp$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_distance(line, line)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_eq(line, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_horizontal(line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_horizontal$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_in(cstring)
 RETURNS line
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_interpt(line, line)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_interpt$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_intersect(line, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_intersect$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_out(line)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_parallel(line, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_parallel$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_perp(line, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_perp$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_recv(internal)
 RETURNS line
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_send(line)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.line_vertical(line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$line_vertical$function$

CREATE OR REPLACE FUNCTION pg_catalog.ln(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_ln$function$

CREATE OR REPLACE FUNCTION pg_catalog.ln(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dlog1$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_close(integer)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_close$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_creat(integer)
 RETURNS oid
 LANGUAGE internal
 STRICT
AS $function$lo_creat$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_create(oid)
 RETURNS oid
 LANGUAGE internal
 STRICT
AS $function$lo_create$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_export(oid, text)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_export$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_from_bytea(oid, bytea)
 RETURNS oid
 LANGUAGE internal
 STRICT
AS $function$lo_from_bytea$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_get(oid, bigint, integer)
 RETURNS bytea
 LANGUAGE internal
 STRICT
AS $function$lo_get_fragment$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_get(oid)
 RETURNS bytea
 LANGUAGE internal
 STRICT
AS $function$lo_get$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_import(text, oid)
 RETURNS oid
 LANGUAGE internal
 STRICT
AS $function$lo_import_with_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_import(text)
 RETURNS oid
 LANGUAGE internal
 STRICT
AS $function$lo_import$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_lseek(integer, integer, integer)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_lseek$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_lseek64(integer, bigint, integer)
 RETURNS bigint
 LANGUAGE internal
 STRICT
AS $function$lo_lseek64$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_open(oid, integer)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_open$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_put(oid, bigint, bytea)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$lo_put$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_tell(integer)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_tell$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_tell64(integer)
 RETURNS bigint
 LANGUAGE internal
 STRICT
AS $function$lo_tell64$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_truncate(integer, integer)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_truncate$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_truncate64(integer, bigint)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_truncate64$function$

CREATE OR REPLACE FUNCTION pg_catalog.lo_unlink(oid)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lo_unlink$function$

CREATE OR REPLACE FUNCTION pg_catalog.log(numeric)
 RETURNS numeric
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.log(10, $1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.log(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_log$function$

CREATE OR REPLACE FUNCTION pg_catalog.log(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dlog10$function$

CREATE OR REPLACE FUNCTION pg_catalog.loread(integer, integer)
 RETURNS bytea
 LANGUAGE internal
 STRICT
AS $function$loread$function$

CREATE OR REPLACE FUNCTION pg_catalog.lower(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lower$function$

CREATE OR REPLACE FUNCTION pg_catalog.lower(anyrange)
 RETURNS anyelement
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_lower$function$

CREATE OR REPLACE FUNCTION pg_catalog.lower_inc(anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_lower_inc$function$

CREATE OR REPLACE FUNCTION pg_catalog.lower_inf(anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_lower_inf$function$

CREATE OR REPLACE FUNCTION pg_catalog.lowrite(integer, bytea)
 RETURNS integer
 LANGUAGE internal
 STRICT
AS $function$lowrite$function$

CREATE OR REPLACE FUNCTION pg_catalog.lpad(text, integer)
 RETURNS text
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.lpad($1, $2, ' ')$function$

CREATE OR REPLACE FUNCTION pg_catalog.lpad(text, integer, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lpad$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg(point, point)
 RETURNS lseg
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_construct$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg(box)
 RETURNS lseg
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_diagonal$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_center(lseg)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_distance(lseg, lseg)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_eq(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$lseg_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_ge(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$lseg_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_gt(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$lseg_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_horizontal(lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_horizontal$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_in(cstring)
 RETURNS lseg
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_interpt(lseg, lseg)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_interpt$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_intersect(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_intersect$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_le(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$lseg_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_length(lseg)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_lt(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$lseg_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_ne(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$lseg_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_out(lseg)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_parallel(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_parallel$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_perp(lseg, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_perp$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_recv(internal)
 RETURNS lseg
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_send(lseg)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.lseg_vertical(lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_vertical$function$

CREATE OR REPLACE FUNCTION pg_catalog.ltrim(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ltrim1$function$

CREATE OR REPLACE FUNCTION pg_catalog.ltrim(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ltrim$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_and(macaddr, macaddr)
 RETURNS macaddr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_and$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_cmp(macaddr, macaddr)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_eq(macaddr, macaddr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$macaddr_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_ge(macaddr, macaddr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$macaddr_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_gt(macaddr, macaddr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$macaddr_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_in(cstring)
 RETURNS macaddr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_le(macaddr, macaddr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$macaddr_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_lt(macaddr, macaddr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$macaddr_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_ne(macaddr, macaddr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$macaddr_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_not(macaddr)
 RETURNS macaddr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_not$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_or(macaddr, macaddr)
 RETURNS macaddr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_or$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_out(macaddr)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_recv(internal)
 RETURNS macaddr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.macaddr_send(macaddr)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.make_date(year integer, month integer, day integer)
 RETURNS date
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$make_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.make_interval(years integer DEFAULT 0, months integer DEFAULT 0, weeks integer DEFAULT 0, days integer DEFAULT 0, hours integer DEFAULT 0, mins integer DEFAULT 0, secs double precision DEFAULT 0.0)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$make_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.make_time(hour integer, min integer, sec double precision)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$make_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.make_timestamp(year integer, month integer, mday integer, hour integer, min integer, sec double precision)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$make_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.make_timestamptz(year integer, month integer, mday integer, hour integer, min integer, sec double precision, timezone text)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$make_timestamptz_at_timezone$function$

CREATE OR REPLACE FUNCTION pg_catalog.make_timestamptz(year integer, month integer, mday integer, hour integer, min integer, sec double precision)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$make_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.makeaclitem(oid, oid, text, boolean)
 RETURNS aclitem
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$makeaclitem$function$

CREATE OR REPLACE FUNCTION pg_catalog.masklen(inet)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_masklen$function$

CREATE OR REPLACE FUNCTION pg_catalog.md5(bytea)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$md5_bytea$function$

CREATE OR REPLACE FUNCTION pg_catalog.md5(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$md5_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_ascii(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/ascii_and_mic', $function$mic_to_ascii$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_big5(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_tw_and_big5', $function$mic_to_big5$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_euc_cn(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_cn_and_mic', $function$mic_to_euc_cn$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_euc_jp(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_jp_and_sjis', $function$mic_to_euc_jp$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_euc_kr(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_kr_and_mic', $function$mic_to_euc_kr$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_euc_tw(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_tw_and_big5', $function$mic_to_euc_tw$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_iso(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$mic_to_iso$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_koi8r(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$mic_to_koi8r$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_latin1(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin_and_mic', $function$mic_to_latin1$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_latin2(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin2_and_win1250', $function$mic_to_latin2$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_latin3(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin_and_mic', $function$mic_to_latin3$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_latin4(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin_and_mic', $function$mic_to_latin4$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_sjis(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_jp_and_sjis', $function$mic_to_sjis$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_win1250(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin2_and_win1250', $function$mic_to_win1250$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_win1251(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$mic_to_win1251$function$

CREATE OR REPLACE FUNCTION pg_catalog.mic_to_win866(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$mic_to_win866$function$

CREATE OR REPLACE FUNCTION pg_catalog.mktinterval(abstime, abstime)
 RETURNS tinterval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$mktinterval$function$

CREATE OR REPLACE FUNCTION pg_catalog.mod(bigint, bigint)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.mod(integer, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.mod(smallint, smallint)
 RETURNS smallint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.mod(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.mode_final(internal, anyelement)
 RETURNS anyelement
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$mode_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.money(numeric)
 RETURNS money
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$numeric_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.money(bigint)
 RETURNS money
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$int8_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.money(integer)
 RETURNS money
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$int4_cash$function$

CREATE OR REPLACE FUNCTION pg_catalog.mul_d_interval(double precision, interval)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$mul_d_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.mxid_age(xid)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$mxid_age$function$

CREATE OR REPLACE FUNCTION pg_catalog.name(character)
 RETURNS name
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpchar_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.name(text)
 RETURNS name
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.name(character varying)
 RETURNS name
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameeq(name, name)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$nameeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.namege(name, name)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$namege$function$

CREATE OR REPLACE FUNCTION pg_catalog.namegt(name, name)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$namegt$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameiclike(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$nameiclike$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameicnlike(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$nameicnlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameicregexeq(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$nameicregexeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameicregexne(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$nameicregexne$function$

CREATE OR REPLACE FUNCTION pg_catalog.namein(cstring)
 RETURNS name
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$namein$function$

CREATE OR REPLACE FUNCTION pg_catalog.namele(name, name)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$namele$function$

CREATE OR REPLACE FUNCTION pg_catalog.namelike(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$namelike$function$

CREATE OR REPLACE FUNCTION pg_catalog.namelt(name, name)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$namelt$function$

CREATE OR REPLACE FUNCTION pg_catalog.namene(name, name)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$namene$function$

CREATE OR REPLACE FUNCTION pg_catalog.namenlike(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$namenlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameout(name)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$nameout$function$

CREATE OR REPLACE FUNCTION pg_catalog.namerecv(internal)
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$namerecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameregexeq(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$nameregexeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.nameregexne(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$nameregexne$function$

CREATE OR REPLACE FUNCTION pg_catalog.namesend(name)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$namesend$function$

CREATE OR REPLACE FUNCTION pg_catalog.neqjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$neqjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.neqsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$neqsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.netmask(inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_netmask$function$

CREATE OR REPLACE FUNCTION pg_catalog.network(inet)
 RETURNS cidr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_network$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_cmp(inet, inet)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_eq(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$network_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_ge(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$network_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_gt(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$network_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_larger(inet, inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_le(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$network_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_lt(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$network_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_ne(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$network_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_overlap(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_overlap$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_smaller(inet, inet)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_sub(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_sub$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_subeq(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_subeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_sup(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_sup$function$

CREATE OR REPLACE FUNCTION pg_catalog.network_supeq(inet, inet)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_supeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.networkjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$networkjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.networksel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$networksel$function$

CREATE OR REPLACE FUNCTION pg_catalog.nextval(regclass)
 RETURNS bigint
 LANGUAGE internal
 STRICT
AS $function$nextval_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.nlikejoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$nlikejoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.nlikesel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$nlikesel$function$

CREATE OR REPLACE FUNCTION pg_catalog.notlike(bytea, bytea)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteanlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.notlike(name, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$namenlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.notlike(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textnlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.now()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$now$function$

CREATE OR REPLACE FUNCTION pg_catalog.npoints(polygon)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_npoints$function$

CREATE OR REPLACE FUNCTION pg_catalog.npoints(path)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_npoints$function$

CREATE OR REPLACE FUNCTION pg_catalog.nth_value(anyelement, integer)
 RETURNS anyelement
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_nth_value$function$

CREATE OR REPLACE FUNCTION pg_catalog.ntile(integer)
 RETURNS integer
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE STRICT
AS $function$window_ntile$function$

CREATE OR REPLACE FUNCTION pg_catalog.num_nonnulls(VARIADIC "any")
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$pg_num_nonnulls$function$

CREATE OR REPLACE FUNCTION pg_catalog.num_nulls(VARIADIC "any")
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$pg_num_nulls$function$

CREATE OR REPLACE FUNCTION pg_catalog."numeric"(money)
 RETURNS numeric
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$cash_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog."numeric"(smallint)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int2_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog."numeric"(bigint)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int8_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog."numeric"(double precision)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog."numeric"(real)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float4_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog."numeric"(integer)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$int4_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog."numeric"(numeric, integer)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_abs(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_abs$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_accum(internal, numeric)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_accum_inv(internal, numeric)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_accum_inv$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_add(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_add$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_avg(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_avg$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_avg_accum(internal, numeric)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_avg_accum$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_avg_combine(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_avg_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_avg_deserialize(bytea, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_avg_deserialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_avg_serialize(internal)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_avg_serialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_cmp(numeric, numeric)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_combine(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_deserialize(bytea, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_deserialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_div(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_div$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_div_trunc(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_div_trunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_eq(numeric, numeric)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_exp(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_exp$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_fac(bigint)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_fac$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_ge(numeric, numeric)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_gt(numeric, numeric)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_in(cstring, oid, integer)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_inc(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_inc$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_larger(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_le(numeric, numeric)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_ln(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_ln$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_log(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_log$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_lt(numeric, numeric)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_mod(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_mod$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_mul(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_ne(numeric, numeric)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_out(numeric)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_avg(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_poly_avg$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_combine(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_poly_combine$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_deserialize(bytea, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_poly_deserialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_serialize(internal)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_poly_serialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_stddev_pop(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_poly_stddev_pop$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_stddev_samp(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_poly_stddev_samp$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_sum(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_poly_sum$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_var_pop(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_poly_var_pop$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_poly_var_samp(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_poly_var_samp$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_power(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_power$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_recv(internal, oid, integer)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_send(numeric)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_serialize(internal)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_serialize$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_smaller(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_sqrt(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_sqrt$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_stddev_pop(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_stddev_pop$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_stddev_samp(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_stddev_samp$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_sub(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_sub$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_sum(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_sum$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_uminus(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_uminus$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_uplus(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_uplus$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_var_pop(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_var_pop$function$

CREATE OR REPLACE FUNCTION pg_catalog.numeric_var_samp(internal)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$numeric_var_samp$function$

CREATE OR REPLACE FUNCTION pg_catalog.numerictypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numerictypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.numerictypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numerictypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.numnode(tsquery)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_numnode$function$

CREATE OR REPLACE FUNCTION pg_catalog.numrange(numeric, numeric, text)
 RETURNS numrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor3$function$

CREATE OR REPLACE FUNCTION pg_catalog.numrange(numeric, numeric)
 RETURNS numrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor2$function$

CREATE OR REPLACE FUNCTION pg_catalog.numrange_subdiff(numeric, numeric)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numrange_subdiff$function$

CREATE OR REPLACE FUNCTION pg_catalog.obj_description(oid)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT
AS $function$select description from pg_catalog.pg_description where objoid = $1 and objsubid = 0$function$

CREATE OR REPLACE FUNCTION pg_catalog.obj_description(oid, name)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT
AS $function$select description from pg_catalog.pg_description where objoid = $1 and classoid = (select oid from pg_catalog.pg_class where relname = $2 and relnamespace = 11) and objsubid = 0$function$

CREATE OR REPLACE FUNCTION pg_catalog.octet_length(bytea)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaoctetlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.octet_length(bit)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitoctetlength$function$

CREATE OR REPLACE FUNCTION pg_catalog.octet_length(character)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bpcharoctetlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.octet_length(text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textoctetlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.oid(bigint)
 RETURNS oid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$i8tooid$function$

CREATE OR REPLACE FUNCTION pg_catalog.oideq(oid, oid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$oideq$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidge(oid, oid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$oidge$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidgt(oid, oid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$oidgt$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidin(cstring)
 RETURNS oid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidin$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidlarger(oid, oid)
 RETURNS oid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidlarger$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidle(oid, oid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$oidle$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidlt(oid, oid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$oidlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidne(oid, oid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$oidne$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidout(oid)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidout$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidrecv(internal)
 RETURNS oid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidsend(oid)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidsmaller(oid, oid)
 RETURNS oid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidsmaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectoreq(oidvector, oidvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectoreq$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorge(oidvector, oidvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorge$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorgt(oidvector, oidvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorgt$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorin(cstring)
 RETURNS oidvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorin$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorle(oidvector, oidvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorle$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorlt(oidvector, oidvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorne(oidvector, oidvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorne$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorout(oidvector)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorout$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorrecv(internal)
 RETURNS oidvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectorsend(oidvector)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$oidvectorsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.oidvectortypes(oidvector)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$oidvectortypes$function$

CREATE OR REPLACE FUNCTION pg_catalog.on_pb(point, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$on_pb$function$

CREATE OR REPLACE FUNCTION pg_catalog.on_pl(point, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$on_pl$function$

CREATE OR REPLACE FUNCTION pg_catalog.on_ppath(point, path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$on_ppath$function$

CREATE OR REPLACE FUNCTION pg_catalog.on_ps(point, lseg)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$on_ps$function$

CREATE OR REPLACE FUNCTION pg_catalog.on_sb(lseg, box)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$on_sb$function$

CREATE OR REPLACE FUNCTION pg_catalog.on_sl(lseg, line)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$on_sl$function$

CREATE OR REPLACE FUNCTION pg_catalog.opaque_in(cstring)
 RETURNS opaque
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$opaque_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.opaque_out(opaque)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$opaque_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.ordered_set_transition(internal, "any")
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$ordered_set_transition$function$

CREATE OR REPLACE FUNCTION pg_catalog.ordered_set_transition_multi(internal, VARIADIC "any")
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$ordered_set_transition_multi$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp without time zone, interval, timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE COST 1
AS $function$select ($1, ($1 + $2)) overlaps ($3, $4)$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp without time zone, timestamp without time zone, timestamp without time zone, interval)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE COST 1
AS $function$select ($1, $2) overlaps ($3, ($3 + $4))$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp without time zone, interval, timestamp without time zone, interval)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE COST 1
AS $function$select ($1, ($1 + $2)) overlaps ($3, ($3 + $4))$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp without time zone, timestamp without time zone, timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$overlaps_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(time without time zone, interval, time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE COST 1
AS $function$select ($1, ($1 + $2)) overlaps ($3, $4)$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(time without time zone, time without time zone, time without time zone, interval)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE COST 1
AS $function$select ($1, $2) overlaps ($3, ($3 + $4))$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(time without time zone, interval, time without time zone, interval)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE COST 1
AS $function$select ($1, ($1 + $2)) overlaps ($3, ($3 + $4))$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(time without time zone, time without time zone, time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$overlaps_time$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp with time zone, interval, timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE sql
 STABLE PARALLEL SAFE COST 1
AS $function$select ($1, ($1 + $2)) overlaps ($3, $4)$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp with time zone, timestamp with time zone, timestamp with time zone, interval)
 RETURNS boolean
 LANGUAGE sql
 STABLE PARALLEL SAFE COST 1
AS $function$select ($1, $2) overlaps ($3, ($3 + $4))$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp with time zone, interval, timestamp with time zone, interval)
 RETURNS boolean
 LANGUAGE sql
 STABLE PARALLEL SAFE COST 1
AS $function$select ($1, ($1 + $2)) overlaps ($3, ($3 + $4))$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(timestamp with time zone, timestamp with time zone, timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$overlaps_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlaps"(time with time zone, time with time zone, time with time zone, time with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$overlaps_timetz$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlay"(bytea, bytea, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaoverlay_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlay"(bytea, bytea, integer, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaoverlay$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlay"(bit, bit, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitoverlay_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlay"(bit, bit, integer, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitoverlay$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlay"(text, text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textoverlay_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog."overlay"(text, text, integer, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textoverlay$function$

CREATE OR REPLACE FUNCTION pg_catalog.parse_ident(str text, strict boolean DEFAULT true)
 RETURNS text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$parse_ident$function$

CREATE OR REPLACE FUNCTION pg_catalog.path(polygon)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_path$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_add(path, path)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_add$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_add_pt(path, point)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_add_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_center(path)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_contain_pt(path, point)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.on_ppath($2, $1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_distance(path, path)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_div_pt(path, point)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_div_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_in(cstring)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_inter(path, path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_inter$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_length(path)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_length$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_mul_pt(path, point)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_mul_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_n_eq(path, path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_n_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_n_ge(path, path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_n_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_n_gt(path, path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_n_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_n_le(path, path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_n_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_n_lt(path, path)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_n_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_npoints(path)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_npoints$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_out(path)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_recv(internal)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_send(path)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.path_sub_pt(path, point)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_sub_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.pclose(path)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_close$function$

create function percent_rank window_percent_rank

CREATE OR REPLACE FUNCTION pg_catalog.percent_rank()
 RETURNS double precision
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE
AS $function$window_percent_rank$function$

CREATE OR REPLACE FUNCTION pg_catalog.percent_rank_final(internal, VARIADIC "any")
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$hypothetical_percent_rank_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.percentile_cont_float8_final(internal, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$percentile_cont_float8_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.percentile_cont_float8_multi_final(internal, double precision[])
 RETURNS double precision[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$percentile_cont_float8_multi_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.percentile_cont_interval_final(internal, double precision)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$percentile_cont_interval_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.percentile_cont_interval_multi_final(internal, double precision[])
 RETURNS interval[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$percentile_cont_interval_multi_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.percentile_disc_final(internal, double precision, anyelement)
 RETURNS anyelement
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$percentile_disc_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.percentile_disc_multi_final(internal, double precision[], anyelement)
 RETURNS anyarray
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$percentile_disc_multi_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_lock(integer, integer)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_lock_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_lock(bigint)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_lock_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_lock_shared(integer, integer)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_lock_shared_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_lock_shared(bigint)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_lock_shared_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_unlock(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_unlock_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_unlock(bigint)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_unlock_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_unlock_all()
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_unlock_all$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_unlock_shared(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_unlock_shared_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_unlock_shared(bigint)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_unlock_shared_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_xact_lock(integer, integer)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_xact_lock_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_xact_lock(bigint)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_xact_lock_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_xact_lock_shared(integer, integer)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_xact_lock_shared_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_advisory_xact_lock_shared(bigint)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_advisory_xact_lock_shared_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_available_extension_versions(OUT name name, OUT version text, OUT superuser boolean, OUT relocatable boolean, OUT schema name, OUT requires name[], OUT comment text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10 ROWS 100
AS $function$pg_available_extension_versions$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_available_extensions(OUT name name, OUT default_version text, OUT comment text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10 ROWS 100
AS $function$pg_available_extensions$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_backend_pid()
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_backend_pid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_backup_start_time()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_backup_start_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_blocking_pids(integer)
 RETURNS integer[]
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_blocking_pids$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_cancel_backend(integer)
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_cancel_backend$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_char_to_encoding(name)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$PG_char_to_encoding$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_client_encoding()
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_client_encoding$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_collation_for("any")
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$pg_collation_for$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_collation_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_collation_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_column_is_updatable(regclass, smallint, boolean)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_column_is_updatable$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_column_size("any")
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_column_size$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_conf_load_time()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_conf_load_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_config(OUT name text, OUT setting text)
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL RESTRICTED STRICT ROWS 23
AS $function$pg_config$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_control_checkpoint(OUT checkpoint_location pg_lsn, OUT prior_location pg_lsn, OUT redo_location pg_lsn, OUT redo_wal_file text, OUT timeline_id integer, OUT prev_timeline_id integer, OUT full_page_writes boolean, OUT next_xid text, OUT next_oid oid, OUT next_multixact_id xid, OUT next_multi_offset xid, OUT oldest_xid xid, OUT oldest_xid_dbid oid, OUT oldest_active_xid xid, OUT oldest_multi_xid xid, OUT oldest_multi_dbid oid, OUT oldest_commit_ts_xid xid, OUT newest_commit_ts_xid xid, OUT checkpoint_time timestamp with time zone)
 RETURNS record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_control_checkpoint$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_control_init(OUT max_data_alignment integer, OUT database_block_size integer, OUT blocks_per_segment integer, OUT wal_block_size integer, OUT bytes_per_wal_segment integer, OUT max_identifier_length integer, OUT max_index_columns integer, OUT max_toast_chunk_size integer, OUT large_object_chunk_size integer, OUT bigint_timestamps boolean, OUT float4_pass_by_value boolean, OUT float8_pass_by_value boolean, OUT data_page_checksum_version integer)
 RETURNS record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_control_init$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_control_recovery(OUT min_recovery_end_location pg_lsn, OUT min_recovery_end_timeline integer, OUT backup_start_location pg_lsn, OUT backup_end_location pg_lsn, OUT end_of_backup_record_required boolean)
 RETURNS record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_control_recovery$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_control_system(OUT pg_control_version integer, OUT catalog_version_no integer, OUT system_identifier bigint, OUT pg_control_last_modified timestamp with time zone)
 RETURNS record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_control_system$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_conversion_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_conversion_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_create_logical_replication_slot(slot_name name, plugin name, OUT slot_name text, OUT xlog_position pg_lsn)
 RETURNS record
 LANGUAGE internal
 STRICT
AS $function$pg_create_logical_replication_slot$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_create_physical_replication_slot(slot_name name, immediately_reserve boolean DEFAULT false, OUT slot_name name, OUT xlog_position pg_lsn)
 RETURNS record
 LANGUAGE internal
 STRICT
AS $function$pg_create_physical_replication_slot$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_create_restore_point(text)
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_create_restore_point$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_current_xlog_flush_location()
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_current_xlog_flush_location$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_current_xlog_insert_location()
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_current_xlog_insert_location$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_current_xlog_location()
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_current_xlog_location$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_cursor(OUT name text, OUT statement text, OUT is_holdable boolean, OUT is_binary boolean, OUT is_scrollable boolean, OUT creation_time timestamp with time zone)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_cursor$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_database_size(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_database_size_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_database_size(name)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_database_size_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ddl_command_in(cstring)
 RETURNS pg_ddl_command
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_ddl_command_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ddl_command_out(pg_ddl_command)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_ddl_command_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ddl_command_recv(internal)
 RETURNS pg_ddl_command
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_ddl_command_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ddl_command_send(pg_ddl_command)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_ddl_command_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_describe_object(oid, oid, integer)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_describe_object$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_drop_replication_slot(name)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_drop_replication_slot$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_encoding_max_length(integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_encoding_max_length_sql$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_encoding_to_char(integer)
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$PG_encoding_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_event_trigger_ddl_commands(OUT classid oid, OUT objid oid, OUT objsubid integer, OUT command_tag text, OUT object_type text, OUT schema_name text, OUT object_identity text, OUT in_extension boolean, OUT command pg_ddl_command)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10 ROWS 100
AS $function$pg_event_trigger_ddl_commands$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_event_trigger_dropped_objects(OUT classid oid, OUT objid oid, OUT objsubid integer, OUT original boolean, OUT normal boolean, OUT is_temporary boolean, OUT object_type text, OUT schema_name text, OUT object_name text, OUT object_identity text, OUT address_names text[], OUT address_args text[])
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10 ROWS 100
AS $function$pg_event_trigger_dropped_objects$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_event_trigger_table_rewrite_oid(OUT oid oid)
 RETURNS oid
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_event_trigger_table_rewrite_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_event_trigger_table_rewrite_reason()
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_event_trigger_table_rewrite_reason$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_export_snapshot()
 RETURNS text
 LANGUAGE internal
 STRICT
AS $function$pg_export_snapshot$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_extension_config_dump(regclass, text)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_extension_config_dump$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_extension_update_paths(name name, OUT source text, OUT target text, OUT path text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10 ROWS 100
AS $function$pg_extension_update_paths$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_filenode_relation(oid, oid)
 RETURNS regclass
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_filenode_relation$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_function_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_function_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_constraintdef(oid, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_constraintdef_ext$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_constraintdef(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_constraintdef$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_expr(pg_node_tree, oid, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_expr_ext$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_expr(pg_node_tree, oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_expr$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_function_arg_default(oid, integer)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_function_arg_default$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_function_arguments(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_function_arguments$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_function_identity_arguments(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_function_identity_arguments$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_function_result(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_function_result$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_functiondef(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_functiondef$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_indexdef(oid, integer, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_indexdef_ext$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_indexdef(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_indexdef$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_keywords(OUT word text, OUT catcode "char", OUT catdesc text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10 ROWS 400
AS $function$pg_get_keywords$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_multixact_members(multixid xid, OUT xid xid, OUT mode text)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_get_multixact_members$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_object_address(type text, name text[], args text[], OUT classid oid, OUT objid oid, OUT subobjid integer)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_object_address$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_replication_slots(OUT slot_name name, OUT plugin name, OUT slot_type text, OUT datoid oid, OUT active boolean, OUT active_pid integer, OUT xmin xid, OUT catalog_xmin xid, OUT restart_lsn pg_lsn, OUT confirmed_flush_lsn pg_lsn)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE ROWS 10
AS $function$pg_get_replication_slots$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_ruledef(oid, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_ruledef_ext$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_ruledef(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_ruledef$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_serial_sequence(text, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_serial_sequence$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_triggerdef(oid, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_triggerdef_ext$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_triggerdef(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_triggerdef$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_userbyid(oid)
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_get_userbyid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_viewdef(oid, integer)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_get_viewdef_wrap$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_viewdef(oid, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_get_viewdef_ext$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_viewdef(text, boolean)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_get_viewdef_name_ext$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_viewdef(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_get_viewdef$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_get_viewdef(text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_get_viewdef_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_has_role(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_has_role_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_has_role(name, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_has_role_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_has_role(oid, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_has_role_id_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_has_role(oid, name, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_has_role_id_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_has_role(name, oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_has_role_name_id$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_has_role(name, name, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_has_role_name_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_identify_object(classid oid, objid oid, subobjid integer, OUT type text, OUT schema text, OUT name text, OUT identity text)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_identify_object$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_identify_object_as_address(classid oid, objid oid, subobjid integer, OUT type text, OUT object_names text[], OUT object_args text[])
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_identify_object_as_address$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_index_column_has_property(regclass, integer, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_index_column_has_property$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_index_has_property(regclass, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_index_has_property$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_indexam_has_property(oid, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_indexam_has_property$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_indexes_size(regclass)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_indexes_size$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_is_in_backup()
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_is_in_backup$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_is_in_recovery()
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_is_in_recovery$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_is_other_temp_schema(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_is_other_temp_schema$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_is_xlog_replay_paused()
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_is_xlog_replay_paused$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_last_committed_xact(OUT xid xid, OUT "timestamp" timestamp with time zone)
 RETURNS record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_last_committed_xact$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_last_xact_replay_timestamp()
 RETURNS timestamp with time zone
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_last_xact_replay_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_last_xlog_receive_location()
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_last_xlog_receive_location$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_last_xlog_replay_location()
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_last_xlog_replay_location$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_listening_channels()
 RETURNS SETOF text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT ROWS 10
AS $function$pg_listening_channels$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lock_status(OUT locktype text, OUT database oid, OUT relation oid, OUT page integer, OUT tuple smallint, OUT virtualxid text, OUT transactionid xid, OUT classid oid, OUT objid oid, OUT objsubid smallint, OUT virtualtransaction text, OUT pid integer, OUT mode text, OUT granted boolean, OUT fastpath boolean)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_lock_status$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_logical_emit_message(boolean, text, bytea)
 RETURNS pg_lsn
 LANGUAGE internal
 STRICT
AS $function$pg_logical_emit_message_bytea$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_logical_emit_message(boolean, text, text)
 RETURNS pg_lsn
 LANGUAGE internal
 STRICT
AS $function$pg_logical_emit_message_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_logical_slot_get_binary_changes(slot_name name, upto_lsn pg_lsn, upto_nchanges integer, VARIADIC options text[] DEFAULT '{}'::text[], OUT location pg_lsn, OUT xid xid, OUT data bytea)
 RETURNS SETOF record
 LANGUAGE internal
 COST 1000
AS $function$pg_logical_slot_get_binary_changes$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_logical_slot_get_changes(slot_name name, upto_lsn pg_lsn, upto_nchanges integer, VARIADIC options text[] DEFAULT '{}'::text[], OUT location pg_lsn, OUT xid xid, OUT data text)
 RETURNS SETOF record
 LANGUAGE internal
 COST 1000
AS $function$pg_logical_slot_get_changes$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_logical_slot_peek_binary_changes(slot_name name, upto_lsn pg_lsn, upto_nchanges integer, VARIADIC options text[] DEFAULT '{}'::text[], OUT location pg_lsn, OUT xid xid, OUT data bytea)
 RETURNS SETOF record
 LANGUAGE internal
 COST 1000
AS $function$pg_logical_slot_peek_binary_changes$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_logical_slot_peek_changes(slot_name name, upto_lsn pg_lsn, upto_nchanges integer, VARIADIC options text[] DEFAULT '{}'::text[], OUT location pg_lsn, OUT xid xid, OUT data text)
 RETURNS SETOF record
 LANGUAGE internal
 COST 1000
AS $function$pg_logical_slot_peek_changes$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ls_dir(text, boolean, boolean)
 RETURNS SETOF text
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_ls_dir$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ls_dir(text)
 RETURNS SETOF text
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_ls_dir_1arg$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_cmp(pg_lsn, pg_lsn)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_eq(pg_lsn, pg_lsn)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_ge(pg_lsn, pg_lsn)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_gt(pg_lsn, pg_lsn)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_hash(pg_lsn)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_hash$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_in(cstring)
 RETURNS pg_lsn
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_le(pg_lsn, pg_lsn)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_lt(pg_lsn, pg_lsn)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_mi(pg_lsn, pg_lsn)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_ne(pg_lsn, pg_lsn)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_out(pg_lsn)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_recv(internal)
 RETURNS pg_lsn
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_lsn_send(pg_lsn)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_lsn_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_my_temp_schema()
 RETURNS oid
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_my_temp_schema$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_node_tree_in(cstring)
 RETURNS pg_node_tree
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_node_tree_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_node_tree_out(pg_node_tree)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_node_tree_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_node_tree_recv(internal)
 RETURNS pg_node_tree
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_node_tree_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_node_tree_send(pg_node_tree)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_node_tree_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_notification_queue_usage()
 RETURNS double precision
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_notification_queue_usage$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_notify(text, text)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED
AS $function$pg_notify$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_opclass_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_opclass_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_operator_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_operator_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_opfamily_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_opfamily_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_options_to_table(options_array text[], OUT option_name text, OUT option_value text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT ROWS 3
AS $function$pg_options_to_table$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_postmaster_start_time()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_postmaster_start_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_prepared_statement(OUT name text, OUT statement text, OUT prepare_time timestamp with time zone, OUT parameter_types regtype[], OUT from_sql boolean)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_prepared_statement$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_prepared_xact(OUT transaction xid, OUT gid text, OUT prepared timestamp with time zone, OUT ownerid oid, OUT dbid oid)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_prepared_xact$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_read_binary_file(text)
 RETURNS bytea
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_read_binary_file_all$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_read_binary_file(text, bigint, bigint)
 RETURNS bytea
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_read_binary_file_off_len$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_read_binary_file(text, bigint, bigint, boolean)
 RETURNS bytea
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_read_binary_file$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_read_file(text)
 RETURNS text
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_read_file_all$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_read_file(text, bigint, bigint, boolean)
 RETURNS text
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_read_file$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_read_file(text, bigint, bigint)
 RETURNS text
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_read_file_off_len$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_relation_filenode(regclass)
 RETURNS oid
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_relation_filenode$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_relation_filepath(regclass)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_relation_filepath$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_relation_is_updatable(regclass, boolean)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_relation_is_updatable$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_relation_size(regclass, text)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_relation_size$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_relation_size(regclass)
 RETURNS bigint
 LANGUAGE sql
 PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.pg_relation_size($1, 'main')$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_reload_conf()
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_reload_conf$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_advance(text, pg_lsn)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_replication_origin_advance$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_create(text)
 RETURNS oid
 LANGUAGE internal
 STRICT
AS $function$pg_replication_origin_create$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_drop(text)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_replication_origin_drop$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_oid(text)
 RETURNS oid
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_replication_origin_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_progress(text, boolean)
 RETURNS pg_lsn
 LANGUAGE internal
 STRICT
AS $function$pg_replication_origin_progress$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_session_is_setup()
 RETURNS boolean
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_replication_origin_session_is_setup$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_session_progress(boolean)
 RETURNS pg_lsn
 LANGUAGE internal
 STRICT
AS $function$pg_replication_origin_session_progress$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_session_reset()
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_replication_origin_session_reset$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_session_setup(text)
 RETURNS void
 LANGUAGE internal
 STRICT
AS $function$pg_replication_origin_session_setup$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_xact_reset()
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_replication_origin_xact_reset$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_replication_origin_xact_setup(pg_lsn, timestamp with time zone)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_replication_origin_xact_setup$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_rotate_logfile()
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_rotate_logfile$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_sequence_parameters(sequence_oid oid, OUT start_value bigint, OUT minimum_value bigint, OUT maximum_value bigint, OUT increment bigint, OUT cycle_option boolean)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_sequence_parameters$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_show_all_file_settings(OUT sourcefile text, OUT sourceline integer, OUT seqno integer, OUT name text, OUT setting text, OUT applied boolean, OUT error text)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$show_all_file_settings$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_show_all_settings(OUT name text, OUT setting text, OUT unit text, OUT category text, OUT short_desc text, OUT extra_desc text, OUT context text, OUT vartype text, OUT source text, OUT min_val text, OUT max_val text, OUT enumvals text[], OUT boot_val text, OUT reset_val text, OUT sourcefile text, OUT sourceline integer, OUT pending_restart boolean)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$show_all_settings$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_show_replication_origin_status(OUT local_id oid, OUT external_id text, OUT remote_lsn pg_lsn, OUT local_lsn pg_lsn)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL RESTRICTED ROWS 100
AS $function$pg_show_replication_origin_status$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_size_bytes(text)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_size_bytes$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_size_pretty(numeric)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_size_pretty_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_size_pretty(bigint)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_size_pretty$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_sleep(double precision)
 RETURNS void
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_sleep$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_sleep_for(interval)
 RETURNS void
 LANGUAGE sql
 PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.pg_sleep(extract(epoch from pg_catalog.clock_timestamp() operator(pg_catalog.+) $1) operator(pg_catalog.-) extract(epoch from pg_catalog.clock_timestamp()))$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_sleep_until(timestamp with time zone)
 RETURNS void
 LANGUAGE sql
 PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.pg_sleep(extract(epoch from $1) operator(pg_catalog.-) extract(epoch from pg_catalog.clock_timestamp()))$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_start_backup(label text, fast boolean DEFAULT false, exclusive boolean DEFAULT true)
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_start_backup$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_clear_snapshot()
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED
AS $function$pg_stat_clear_snapshot$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_file(filename text, missing_ok boolean, OUT size bigint, OUT access timestamp with time zone, OUT modification timestamp with time zone, OUT change timestamp with time zone, OUT creation timestamp with time zone, OUT isdir boolean)
 RETURNS record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_stat_file$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_file(filename text, OUT size bigint, OUT access timestamp with time zone, OUT modification timestamp with time zone, OUT change timestamp with time zone, OUT creation timestamp with time zone, OUT isdir boolean)
 RETURNS record
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_stat_file_1arg$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_activity(pid integer, OUT datid oid, OUT pid integer, OUT usesysid oid, OUT application_name text, OUT state text, OUT query text, OUT wait_event_type text, OUT wait_event text, OUT xact_start timestamp with time zone, OUT query_start timestamp with time zone, OUT backend_start timestamp with time zone, OUT state_change timestamp with time zone, OUT client_addr inet, OUT client_hostname text, OUT client_port integer, OUT backend_xid xid, OUT backend_xmin xid, OUT ssl boolean, OUT sslversion text, OUT sslcipher text, OUT sslbits integer, OUT sslcompression boolean, OUT sslclientdn text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED ROWS 100
AS $function$pg_stat_get_activity$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_analyze_count(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_analyze_count$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_archiver(OUT archived_count bigint, OUT last_archived_wal text, OUT last_archived_time timestamp with time zone, OUT failed_count bigint, OUT last_failed_wal text, OUT last_failed_time timestamp with time zone, OUT stats_reset timestamp with time zone)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED
AS $function$pg_stat_get_archiver$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_autoanalyze_count(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_autoanalyze_count$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_autovacuum_count(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_autovacuum_count$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_activity(integer)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_activity$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_activity_start(integer)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_activity_start$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_client_addr(integer)
 RETURNS inet
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_client_addr$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_client_port(integer)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_client_port$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_dbid(integer)
 RETURNS oid
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_dbid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_idset()
 RETURNS SETOF integer
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT ROWS 100
AS $function$pg_stat_get_backend_idset$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_pid(integer)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_pid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_start(integer)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_start$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_userid(integer)
 RETURNS oid
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_userid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_wait_event(integer)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_wait_event$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_wait_event_type(integer)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_wait_event_type$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_backend_xact_start(integer)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_backend_xact_start$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_bgwriter_buf_written_checkpoints()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_bgwriter_buf_written_checkpoints$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_bgwriter_buf_written_clean()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_bgwriter_buf_written_clean$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_bgwriter_maxwritten_clean()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_bgwriter_maxwritten_clean$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_bgwriter_requested_checkpoints()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_bgwriter_requested_checkpoints$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_bgwriter_stat_reset_time()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_bgwriter_stat_reset_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_bgwriter_timed_checkpoints()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_bgwriter_timed_checkpoints$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_blocks_fetched(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_blocks_fetched$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_blocks_hit(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_blocks_hit$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_buf_alloc()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_buf_alloc$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_buf_fsync_backend()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_buf_fsync_backend$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_buf_written_backend()
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_buf_written_backend$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_checkpoint_sync_time()
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_checkpoint_sync_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_checkpoint_write_time()
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_checkpoint_write_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_blk_read_time(oid)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_blk_read_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_blk_write_time(oid)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_blk_write_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_blocks_fetched(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_blocks_fetched$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_blocks_hit(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_blocks_hit$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_conflict_all(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_conflict_all$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_conflict_bufferpin(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_conflict_bufferpin$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_conflict_lock(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_conflict_lock$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_conflict_snapshot(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_conflict_snapshot$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_conflict_startup_deadlock(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_conflict_startup_deadlock$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_conflict_tablespace(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_conflict_tablespace$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_deadlocks(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_deadlocks$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_numbackends(oid)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_numbackends$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_stat_reset_time(oid)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_stat_reset_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_temp_bytes(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_temp_bytes$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_temp_files(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_temp_files$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_tuples_deleted(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_tuples_deleted$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_tuples_fetched(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_tuples_fetched$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_tuples_inserted(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_tuples_inserted$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_tuples_returned(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_tuples_returned$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_tuples_updated(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_tuples_updated$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_xact_commit(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_xact_commit$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_db_xact_rollback(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_db_xact_rollback$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_dead_tuples(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_dead_tuples$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_function_calls(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_function_calls$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_function_self_time(oid)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_function_self_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_function_total_time(oid)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_function_total_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_last_analyze_time(oid)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_last_analyze_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_last_autoanalyze_time(oid)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_last_autoanalyze_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_last_autovacuum_time(oid)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_last_autovacuum_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_last_vacuum_time(oid)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_last_vacuum_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_live_tuples(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_live_tuples$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_mod_since_analyze(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_mod_since_analyze$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_numscans(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_numscans$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_progress_info(cmdtype text, OUT pid integer, OUT datid oid, OUT relid oid, OUT param1 bigint, OUT param2 bigint, OUT param3 bigint, OUT param4 bigint, OUT param5 bigint, OUT param6 bigint, OUT param7 bigint, OUT param8 bigint, OUT param9 bigint, OUT param10 bigint)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT ROWS 100
AS $function$pg_stat_get_progress_info$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_snapshot_timestamp()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_snapshot_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_tuples_deleted(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_tuples_deleted$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_tuples_fetched(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_tuples_fetched$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_tuples_hot_updated(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_tuples_hot_updated$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_tuples_inserted(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_tuples_inserted$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_tuples_returned(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_tuples_returned$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_tuples_updated(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_tuples_updated$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_vacuum_count(oid)
 RETURNS bigint
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_vacuum_count$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_wal_receiver(OUT pid integer, OUT status text, OUT receive_start_lsn pg_lsn, OUT receive_start_tli integer, OUT received_lsn pg_lsn, OUT received_tli integer, OUT last_msg_send_time timestamp with time zone, OUT last_msg_receipt_time timestamp with time zone, OUT latest_end_lsn pg_lsn, OUT latest_end_time timestamp with time zone, OUT slot_name text, OUT conninfo text)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED
AS $function$pg_stat_get_wal_receiver$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_wal_senders(OUT pid integer, OUT state text, OUT sent_location pg_lsn, OUT write_location pg_lsn, OUT flush_location pg_lsn, OUT replay_location pg_lsn, OUT sync_priority integer, OUT sync_state text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED ROWS 10
AS $function$pg_stat_get_wal_senders$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_blocks_fetched(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_blocks_fetched$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_blocks_hit(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_blocks_hit$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_function_calls(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_function_calls$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_function_self_time(oid)
 RETURNS double precision
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_function_self_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_function_total_time(oid)
 RETURNS double precision
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_function_total_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_numscans(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_numscans$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_tuples_deleted(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_tuples_deleted$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_tuples_fetched(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_tuples_fetched$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_tuples_hot_updated(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_tuples_hot_updated$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_tuples_inserted(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_tuples_inserted$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_tuples_returned(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_tuples_returned$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_get_xact_tuples_updated(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$pg_stat_get_xact_tuples_updated$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_reset()
 RETURNS void
 LANGUAGE internal
 PARALLEL SAFE
AS $function$pg_stat_reset$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_reset_shared(text)
 RETURNS void
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_stat_reset_shared$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_reset_single_function_counters(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_stat_reset_single_function_counters$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stat_reset_single_table_counters(oid)
 RETURNS void
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_stat_reset_single_table_counters$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stop_backup(exclusive boolean, OUT lsn pg_lsn, OUT labelfile text, OUT spcmapfile text)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL SAFE STRICT ROWS 1
AS $function$pg_stop_backup_v2$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_stop_backup()
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_stop_backup$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_switch_xlog()
 RETURNS pg_lsn
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_switch_xlog$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_table_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_table_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_table_size(regclass)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_table_size$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_tablespace_databases(oid)
 RETURNS SETOF oid
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_tablespace_databases$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_tablespace_location(oid)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_tablespace_location$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_tablespace_size(name)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_tablespace_size_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_tablespace_size(oid)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_tablespace_size_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_terminate_backend(integer)
 RETURNS boolean
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_terminate_backend$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_timezone_abbrevs(OUT abbrev text, OUT utc_offset interval, OUT is_dst boolean)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_timezone_abbrevs$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_timezone_names(OUT name text, OUT abbrev text, OUT utc_offset interval, OUT is_dst boolean)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_timezone_names$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_total_relation_size(regclass)
 RETURNS bigint
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_total_relation_size$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_trigger_depth()
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pg_trigger_depth$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_lock(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_lock_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_lock(bigint)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_lock_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_lock_shared(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_lock_shared_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_lock_shared(bigint)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_lock_shared_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_xact_lock(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_xact_lock_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_xact_lock(bigint)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_xact_lock_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_xact_lock_shared(integer, integer)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_xact_lock_shared_int4$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_try_advisory_xact_lock_shared(bigint)
 RETURNS boolean
 LANGUAGE internal
 STRICT
AS $function$pg_try_advisory_xact_lock_shared_int8$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ts_config_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_ts_config_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ts_dict_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_ts_dict_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ts_parser_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_ts_parser_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_ts_template_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_ts_template_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_type_is_visible(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 10
AS $function$pg_type_is_visible$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_typeof("any")
 RETURNS regtype
 LANGUAGE internal
 STABLE PARALLEL SAFE
AS $function$pg_typeof$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_xact_commit_timestamp(xid)
 RETURNS timestamp with time zone
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_xact_commit_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_xlog_location_diff(pg_lsn, pg_lsn)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_xlog_location_diff$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_xlog_replay_pause()
 RETURNS void
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_xlog_replay_pause$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_xlog_replay_resume()
 RETURNS void
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$pg_xlog_replay_resume$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_xlogfile_name(pg_lsn)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_xlogfile_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.pg_xlogfile_name_offset(wal_location pg_lsn, OUT file_name text, OUT file_offset integer)
 RETURNS record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pg_xlogfile_name_offset$function$

CREATE OR REPLACE FUNCTION pg_catalog.phraseto_tsquery(regconfig, text)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT COST 100
AS $function$phraseto_tsquery_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.phraseto_tsquery(text)
 RETURNS tsquery
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$phraseto_tsquery$function$

CREATE OR REPLACE FUNCTION pg_catalog.pi()
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dpi$function$

CREATE OR REPLACE FUNCTION pg_catalog.plainto_tsquery(text)
 RETURNS tsquery
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$plainto_tsquery$function$

CREATE OR REPLACE FUNCTION pg_catalog.plainto_tsquery(regconfig, text)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT COST 100
AS $function$plainto_tsquery_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.plpgsql_call_handler()
 RETURNS language_handler
 LANGUAGE c
AS '$libdir/plpgsql', $function$plpgsql_call_handler$function$

CREATE OR REPLACE FUNCTION pg_catalog.plpgsql_inline_handler(internal)
 RETURNS void
 LANGUAGE c
 STRICT
AS '$libdir/plpgsql', $function$plpgsql_inline_handler$function$

CREATE OR REPLACE FUNCTION pg_catalog.plpgsql_validator(oid)
 RETURNS void
 LANGUAGE c
 STRICT
AS '$libdir/plpgsql', $function$plpgsql_validator$function$

CREATE OR REPLACE FUNCTION pg_catalog.point(polygon)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.point(box)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.point(path)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.point(lseg)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$lseg_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.point(double precision, double precision)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$construct_point$function$

CREATE OR REPLACE FUNCTION pg_catalog.point(circle)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_above(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_above$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_add(point, point)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_add$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_below(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_below$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_distance(point, point)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_div(point, point)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_div$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_eq(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_horiz(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_horiz$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_in(cstring)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_left(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_left$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_mul(point, point)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_mul$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_ne(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_out(point)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_recv(internal)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_right(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_right$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_send(point)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_sub(point, point)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_sub$function$

CREATE OR REPLACE FUNCTION pg_catalog.point_vert(point, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_vert$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_above(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_above$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_below(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_below$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_center(polygon)
 RETURNS point
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_center$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_contain(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_contain$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_contain_pt(polygon, point)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_contain_pt$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_contained(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_contained$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_distance(polygon, polygon)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_in(cstring)
 RETURNS polygon
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_left(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_left$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_npoints(polygon)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_npoints$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_out(polygon)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_overabove(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_overabove$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_overbelow(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_overbelow$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_overlap(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_overlap$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_overleft(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_overleft$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_overright(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_overright$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_recv(internal)
 RETURNS polygon
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_right(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_right$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_same(polygon, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.poly_send(polygon)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$poly_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.polygon(circle)
 RETURNS polygon
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.polygon(12, $1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.polygon(integer, circle)
 RETURNS polygon
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_poly$function$

CREATE OR REPLACE FUNCTION pg_catalog.polygon(path)
 RETURNS polygon
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_poly$function$

CREATE OR REPLACE FUNCTION pg_catalog.polygon(box)
 RETURNS polygon
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_poly$function$

CREATE OR REPLACE FUNCTION pg_catalog.popen(path)
 RETURNS path
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$path_open$function$

CREATE OR REPLACE FUNCTION pg_catalog."position"(text, text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textpos$function$

CREATE OR REPLACE FUNCTION pg_catalog."position"(bytea, bytea)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteapos$function$

CREATE OR REPLACE FUNCTION pg_catalog."position"(bit, bit)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitposition$function$

CREATE OR REPLACE FUNCTION pg_catalog.positionjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$positionjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.positionsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$positionsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.postgresql_fdw_validator(text[], oid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$postgresql_fdw_validator$function$

CREATE OR REPLACE FUNCTION pg_catalog.pow(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_power$function$

CREATE OR REPLACE FUNCTION pg_catalog.pow(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dpow$function$

CREATE OR REPLACE FUNCTION pg_catalog.power(numeric, numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_power$function$

CREATE OR REPLACE FUNCTION pg_catalog.power(double precision, double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dpow$function$

CREATE OR REPLACE FUNCTION pg_catalog.prsd_end(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$prsd_end$function$

CREATE OR REPLACE FUNCTION pg_catalog.prsd_headline(internal, internal, tsquery)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$prsd_headline$function$

CREATE OR REPLACE FUNCTION pg_catalog.prsd_lextype(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$prsd_lextype$function$

CREATE OR REPLACE FUNCTION pg_catalog.prsd_nexttoken(internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$prsd_nexttoken$function$

CREATE OR REPLACE FUNCTION pg_catalog.prsd_start(internal, integer)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$prsd_start$function$

CREATE OR REPLACE FUNCTION pg_catalog.pt_contained_circle(point, circle)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pt_contained_circle$function$

CREATE OR REPLACE FUNCTION pg_catalog.pt_contained_poly(point, polygon)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$pt_contained_poly$function$

CREATE OR REPLACE FUNCTION pg_catalog.query_to_xml(query text, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE STRICT COST 100
AS $function$query_to_xml$function$

CREATE OR REPLACE FUNCTION pg_catalog.query_to_xml_and_xmlschema(query text, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE STRICT COST 100
AS $function$query_to_xml_and_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.query_to_xmlschema(query text, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE STRICT COST 100
AS $function$query_to_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.querytree(tsquery)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquerytree$function$

CREATE OR REPLACE FUNCTION pg_catalog.quote_ident(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$quote_ident$function$

CREATE OR REPLACE FUNCTION pg_catalog.quote_literal(anyelement)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.quote_literal($1::pg_catalog.text)$function$

CREATE OR REPLACE FUNCTION pg_catalog.quote_literal(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$quote_literal$function$

CREATE OR REPLACE FUNCTION pg_catalog.quote_nullable(anyelement)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE COST 1
AS $function$select pg_catalog.quote_nullable($1::pg_catalog.text)$function$

CREATE OR REPLACE FUNCTION pg_catalog.quote_nullable(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$quote_nullable$function$

CREATE OR REPLACE FUNCTION pg_catalog.radians(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$radians$function$

CREATE OR REPLACE FUNCTION pg_catalog.radius(circle)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$circle_radius$function$

CREATE OR REPLACE FUNCTION pg_catalog.random()
 RETURNS double precision
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$drandom$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_adjacent(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_adjacent$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_after(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_after$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_before(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_before$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_cmp(anyrange, anyrange)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_contained_by(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_contained_by$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_contains(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_contains$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_contains_elem(anyrange, anyelement)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_contains_elem$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_eq(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_ge(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_compress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_compress$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_consistent(internal, anyrange, smallint, oid, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_decompress(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_decompress$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_fetch(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_fetch$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_penalty(internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_penalty$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_picksplit(internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_same(anyrange, anyrange, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_same$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gist_union(internal, internal)
 RETURNS anyrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gist_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_gt(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_in(cstring, oid, integer)
 RETURNS anyrange
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$range_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_intersect(anyrange, anyrange)
 RETURNS anyrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_intersect$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_le(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_lt(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_merge(anyrange, anyrange)
 RETURNS anyrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_merge$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_minus(anyrange, anyrange)
 RETURNS anyrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_minus$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_ne(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_out(anyrange)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$range_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_overlaps(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_overlaps$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_overleft(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_overleft$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_overright(anyrange, anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_overright$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_recv(internal, oid, integer)
 RETURNS anyrange
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$range_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_send(anyrange)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$range_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_typanalyze(internal)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$range_typanalyze$function$

CREATE OR REPLACE FUNCTION pg_catalog.range_union(anyrange, anyrange)
 RETURNS anyrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_union$function$

CREATE OR REPLACE FUNCTION pg_catalog.rangesel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$rangesel$function$

create function rank window_rank

CREATE OR REPLACE FUNCTION pg_catalog.rank()
 RETURNS bigint
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE
AS $function$window_rank$function$

CREATE OR REPLACE FUNCTION pg_catalog.rank_final(internal, VARIADIC "any")
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$hypothetical_rank_final$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_eq(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_ge(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_gt(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_image_eq(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_image_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_image_ge(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_image_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_image_gt(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_image_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_image_le(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_image_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_image_lt(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_image_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_image_ne(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_image_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_in(cstring, oid, integer)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$record_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_le(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_lt(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_ne(record, record)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$record_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_out(record)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$record_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_recv(internal, oid, integer)
 RETURNS record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$record_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.record_send(record)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$record_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.regclass(text)
 RETURNS regclass
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$text_regclass$function$

CREATE OR REPLACE FUNCTION pg_catalog.regclassin(cstring)
 RETURNS regclass
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regclassin$function$

CREATE OR REPLACE FUNCTION pg_catalog.regclassout(regclass)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regclassout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regclassrecv(internal)
 RETURNS regclass
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regclassrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regclasssend(regclass)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regclasssend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regconfigin(cstring)
 RETURNS regconfig
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regconfigin$function$

CREATE OR REPLACE FUNCTION pg_catalog.regconfigout(regconfig)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regconfigout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regconfigrecv(internal)
 RETURNS regconfig
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regconfigrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regconfigsend(regconfig)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regconfigsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regdictionaryin(cstring)
 RETURNS regdictionary
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regdictionaryin$function$

CREATE OR REPLACE FUNCTION pg_catalog.regdictionaryout(regdictionary)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regdictionaryout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regdictionaryrecv(internal)
 RETURNS regdictionary
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regdictionaryrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regdictionarysend(regdictionary)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regdictionarysend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexeqjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regexeqjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexeqsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regexeqsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexnejoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regexnejoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexnesel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regexnesel$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_matches(text, text, text)
 RETURNS SETOF text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 10
AS $function$regexp_matches$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_matches(text, text)
 RETURNS SETOF text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 1
AS $function$regexp_matches_no_flags$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_replace(text, text, text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textregexreplace$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_replace(text, text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textregexreplace_noopt$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_split_to_array(text, text, text)
 RETURNS text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regexp_split_to_array$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_split_to_array(text, text)
 RETURNS text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regexp_split_to_array_no_flags$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_split_to_table(text, text, text)
 RETURNS SETOF text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regexp_split_to_table$function$

CREATE OR REPLACE FUNCTION pg_catalog.regexp_split_to_table(text, text)
 RETURNS SETOF text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regexp_split_to_table_no_flags$function$

CREATE OR REPLACE FUNCTION pg_catalog.regnamespacein(cstring)
 RETURNS regnamespace
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regnamespacein$function$

CREATE OR REPLACE FUNCTION pg_catalog.regnamespaceout(regnamespace)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regnamespaceout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regnamespacerecv(internal)
 RETURNS regnamespace
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regnamespacerecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regnamespacesend(regnamespace)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regnamespacesend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regoperatorin(cstring)
 RETURNS regoperator
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regoperatorin$function$

CREATE OR REPLACE FUNCTION pg_catalog.regoperatorout(regoperator)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regoperatorout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regoperatorrecv(internal)
 RETURNS regoperator
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regoperatorrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regoperatorsend(regoperator)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regoperatorsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regoperin(cstring)
 RETURNS regoper
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regoperin$function$

CREATE OR REPLACE FUNCTION pg_catalog.regoperout(regoper)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regoperout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regoperrecv(internal)
 RETURNS regoper
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regoperrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regopersend(regoper)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regopersend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regprocedurein(cstring)
 RETURNS regprocedure
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regprocedurein$function$

CREATE OR REPLACE FUNCTION pg_catalog.regprocedureout(regprocedure)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regprocedureout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regprocedurerecv(internal)
 RETURNS regprocedure
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regprocedurerecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regproceduresend(regprocedure)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regproceduresend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regprocin(cstring)
 RETURNS regproc
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regprocin$function$

CREATE OR REPLACE FUNCTION pg_catalog.regprocout(regproc)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regprocout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regprocrecv(internal)
 RETURNS regproc
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regprocrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regprocsend(regproc)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regprocsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regrolein(cstring)
 RETURNS regrole
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regrolein$function$

CREATE OR REPLACE FUNCTION pg_catalog.regroleout(regrole)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regroleout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regrolerecv(internal)
 RETURNS regrole
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regrolerecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regrolesend(regrole)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regrolesend$function$

CREATE OR REPLACE FUNCTION pg_catalog.regtypein(cstring)
 RETURNS regtype
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regtypein$function$

CREATE OR REPLACE FUNCTION pg_catalog.regtypeout(regtype)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$regtypeout$function$

CREATE OR REPLACE FUNCTION pg_catalog.regtyperecv(internal)
 RETURNS regtype
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regtyperecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.regtypesend(regtype)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$regtypesend$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltime(interval)
 RETURNS reltime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_reltime$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimeeq(reltime, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$reltimeeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimege(reltime, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$reltimege$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimegt(reltime, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$reltimegt$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimein(cstring)
 RETURNS reltime
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$reltimein$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimele(reltime, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$reltimele$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimelt(reltime, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$reltimelt$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimene(reltime, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$reltimene$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimeout(reltime)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$reltimeout$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimerecv(internal)
 RETURNS reltime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$reltimerecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.reltimesend(reltime)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$reltimesend$function$

CREATE OR REPLACE FUNCTION pg_catalog.repeat(text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$repeat$function$

CREATE OR REPLACE FUNCTION pg_catalog.replace(text, text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$replace_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.reverse(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_reverse$function$

CREATE OR REPLACE FUNCTION pg_catalog."right"(text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_right$function$

CREATE OR REPLACE FUNCTION pg_catalog.round(numeric)
 RETURNS numeric
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.round($1,0)$function$

CREATE OR REPLACE FUNCTION pg_catalog.round(numeric, integer)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_round$function$

CREATE OR REPLACE FUNCTION pg_catalog.round(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dround$function$

CREATE OR REPLACE FUNCTION pg_catalog.row_number()
 RETURNS bigint
 LANGUAGE internal
 WINDOW IMMUTABLE PARALLEL SAFE
AS $function$window_row_number$function$

CREATE OR REPLACE FUNCTION pg_catalog.row_security_active(text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$row_security_active_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.row_security_active(oid)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$row_security_active$function$

CREATE OR REPLACE FUNCTION pg_catalog.row_to_json(record, boolean)
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$row_to_json_pretty$function$

CREATE OR REPLACE FUNCTION pg_catalog.row_to_json(record)
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$row_to_json$function$

CREATE OR REPLACE FUNCTION pg_catalog.rpad(text, integer)
 RETURNS text
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.rpad($1, $2, ' ')$function$

CREATE OR REPLACE FUNCTION pg_catalog.rpad(text, integer, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$rpad$function$

CREATE OR REPLACE FUNCTION pg_catalog.rtrim(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$rtrim1$function$

CREATE OR REPLACE FUNCTION pg_catalog.rtrim(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$rtrim$function$

CREATE OR REPLACE FUNCTION pg_catalog.scalargtjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$scalargtjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.scalargtsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$scalargtsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.scalarltjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$scalarltjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.scalarltsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$scalarltsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.scale(numeric)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_scale$function$

CREATE OR REPLACE FUNCTION pg_catalog.schema_to_xml(schema name, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$schema_to_xml$function$

CREATE OR REPLACE FUNCTION pg_catalog.schema_to_xml_and_xmlschema(schema name, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$schema_to_xml_and_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.schema_to_xmlschema(schema name, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$schema_to_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog."session_user"()
 RETURNS name
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$session_user$function$

CREATE OR REPLACE FUNCTION pg_catalog.set_bit(bytea, integer, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaSetBit$function$

CREATE OR REPLACE FUNCTION pg_catalog.set_bit(bit, integer, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitsetbit$function$

CREATE OR REPLACE FUNCTION pg_catalog.set_byte(bytea, integer, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$byteaSetByte$function$

CREATE OR REPLACE FUNCTION pg_catalog.set_config(text, text, boolean)
 RETURNS text
 LANGUAGE internal
AS $function$set_config_by_name$function$

CREATE OR REPLACE FUNCTION pg_catalog.set_masklen(cidr, integer)
 RETURNS cidr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$cidr_set_masklen$function$

CREATE OR REPLACE FUNCTION pg_catalog.set_masklen(inet, integer)
 RETURNS inet
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$inet_set_masklen$function$

CREATE OR REPLACE FUNCTION pg_catalog.setseed(double precision)
 RETURNS void
 LANGUAGE internal
 PARALLEL RESTRICTED STRICT
AS $function$setseed$function$

CREATE OR REPLACE FUNCTION pg_catalog.setval(regclass, bigint, boolean)
 RETURNS bigint
 LANGUAGE internal
 STRICT
AS $function$setval3_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.setval(regclass, bigint)
 RETURNS bigint
 LANGUAGE internal
 STRICT
AS $function$setval_oid$function$

CREATE OR REPLACE FUNCTION pg_catalog.setweight(tsvector, "char")
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_setweight$function$

CREATE OR REPLACE FUNCTION pg_catalog.setweight(tsvector, "char", text[])
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_setweight_by_filter$function$

CREATE OR REPLACE FUNCTION pg_catalog.shell_in(cstring)
 RETURNS opaque
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$shell_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.shell_out(opaque)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$shell_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.shift_jis_2004_to_euc_jis_2004(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc2004_sjis2004', $function$shift_jis_2004_to_euc_jis_2004$function$

CREATE OR REPLACE FUNCTION pg_catalog.shift_jis_2004_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_sjis2004', $function$shift_jis_2004_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.shobj_description(oid, name)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT
AS $function$select description from pg_catalog.pg_shdescription where objoid = $1 and classoid = (select oid from pg_catalog.pg_class where relname = $2 and relnamespace = 11)$function$

CREATE OR REPLACE FUNCTION pg_catalog.sign(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsign$function$

CREATE OR REPLACE FUNCTION pg_catalog.sign(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_sign$function$

CREATE OR REPLACE FUNCTION pg_catalog.similar_escape(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$similar_escape$function$

CREATE OR REPLACE FUNCTION pg_catalog.sin(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsin$function$

CREATE OR REPLACE FUNCTION pg_catalog.sind(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsind$function$

CREATE OR REPLACE FUNCTION pg_catalog.sjis_to_euc_jp(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_jp_and_sjis', $function$sjis_to_euc_jp$function$

CREATE OR REPLACE FUNCTION pg_catalog.sjis_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/euc_jp_and_sjis', $function$sjis_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.sjis_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_sjis', $function$sjis_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.slope(point, point)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$point_slope$function$

CREATE OR REPLACE FUNCTION pg_catalog.smgreq(smgr, smgr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$smgreq$function$

CREATE OR REPLACE FUNCTION pg_catalog.smgrin(cstring)
 RETURNS smgr
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$smgrin$function$

CREATE OR REPLACE FUNCTION pg_catalog.smgrne(smgr, smgr)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$smgrne$function$

CREATE OR REPLACE FUNCTION pg_catalog.smgrout(smgr)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$smgrout$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_box_quad_choose(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_box_quad_choose$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_box_quad_config(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_box_quad_config$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_box_quad_inner_consistent(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_box_quad_inner_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_box_quad_leaf_consistent(internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_box_quad_leaf_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_box_quad_picksplit(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_box_quad_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_kd_choose(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_kd_choose$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_kd_config(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_kd_config$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_kd_inner_consistent(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_kd_inner_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_kd_picksplit(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_kd_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_quad_choose(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_quad_choose$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_quad_config(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_quad_config$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_quad_inner_consistent(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_quad_inner_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_quad_leaf_consistent(internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_quad_leaf_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_quad_picksplit(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_quad_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_range_quad_choose(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_range_quad_choose$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_range_quad_config(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_range_quad_config$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_range_quad_inner_consistent(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_range_quad_inner_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_range_quad_leaf_consistent(internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_range_quad_leaf_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_range_quad_picksplit(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_range_quad_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_text_choose(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_text_choose$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_text_config(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_text_config$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_text_inner_consistent(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_text_inner_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_text_leaf_consistent(internal, internal)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_text_leaf_consistent$function$

CREATE OR REPLACE FUNCTION pg_catalog.spg_text_picksplit(internal, internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$spg_text_picksplit$function$

CREATE OR REPLACE FUNCTION pg_catalog.spghandler(internal)
 RETURNS index_am_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$spghandler$function$

CREATE OR REPLACE FUNCTION pg_catalog.split_part(text, text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$split_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.sqrt(numeric)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_sqrt$function$

CREATE OR REPLACE FUNCTION pg_catalog.sqrt(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dsqrt$function$

CREATE OR REPLACE FUNCTION pg_catalog.statement_timestamp()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT
AS $function$statement_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.string_agg_finalfn(internal)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$string_agg_finalfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.string_agg_transfn(internal, text, text)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$string_agg_transfn$function$

CREATE OR REPLACE FUNCTION pg_catalog.string_to_array(text, text)
 RETURNS text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$text_to_array$function$

CREATE OR REPLACE FUNCTION pg_catalog.string_to_array(text, text, text)
 RETURNS text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$text_to_array_null$function$

CREATE OR REPLACE FUNCTION pg_catalog.strip(tsvector)
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_strip$function$

CREATE OR REPLACE FUNCTION pg_catalog.strpos(text, text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textpos$function$

CREATE OR REPLACE FUNCTION pg_catalog.substr(text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_substr_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog.substr(text, integer, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_substr$function$

CREATE OR REPLACE FUNCTION pg_catalog.substr(bytea, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytea_substr_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog.substr(bytea, integer, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytea_substr$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_substr_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(text, integer, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_substr$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(text, text, text)
 RETURNS text
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.substring($1, pg_catalog.similar_escape($2, $3))$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textregexsubstr$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(bytea, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytea_substr_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(bytea, integer, integer)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bytea_substr$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(bit, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitsubstr_no_len$function$

CREATE OR REPLACE FUNCTION pg_catalog."substring"(bit, integer, integer)
 RETURNS bit
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitsubstr$function$

CREATE OR REPLACE FUNCTION pg_catalog.suppress_redundant_updates_trigger()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$suppress_redundant_updates_trigger$function$

CREATE OR REPLACE FUNCTION pg_catalog.system(internal)
 RETURNS tsm_handler
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$tsm_system_handler$function$

CREATE OR REPLACE FUNCTION pg_catalog.table_to_xml(tbl regclass, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$table_to_xml$function$

CREATE OR REPLACE FUNCTION pg_catalog.table_to_xml_and_xmlschema(tbl regclass, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$table_to_xml_and_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.table_to_xmlschema(tbl regclass, nulls boolean, tableforest boolean, targetns text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL RESTRICTED STRICT COST 100
AS $function$table_to_xmlschema$function$

CREATE OR REPLACE FUNCTION pg_catalog.tan(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtan$function$

CREATE OR REPLACE FUNCTION pg_catalog.tand(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtand$function$

CREATE OR REPLACE FUNCTION pg_catalog.text("char")
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$char_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.text(inet)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$network_show$function$

CREATE OR REPLACE FUNCTION pg_catalog.text(name)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$name_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.text(character)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$rtrim1$function$

CREATE OR REPLACE FUNCTION pg_catalog.text(boolean)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$booltext$function$

CREATE OR REPLACE FUNCTION pg_catalog.text(xml)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xmltotext$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_ge(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_gt(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_larger(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_le(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_lt(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_pattern_ge(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_pattern_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_pattern_gt(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_pattern_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_pattern_le(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_pattern_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_pattern_lt(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_pattern_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.text_smaller(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$text_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.textanycat(text, anynonarray)
 RETURNS text
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select $1 || $2::pg_catalog.text$function$

CREATE OR REPLACE FUNCTION pg_catalog.textcat(text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textcat$function$

CREATE OR REPLACE FUNCTION pg_catalog.texteq(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$texteq$function$

CREATE OR REPLACE FUNCTION pg_catalog.texticlike(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.texticnlike(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticnlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.texticregexeq(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticregexeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.texticregexne(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$texticregexne$function$

CREATE OR REPLACE FUNCTION pg_catalog.textin(cstring)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textin$function$

CREATE OR REPLACE FUNCTION pg_catalog.textlen(text)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textlen$function$

CREATE OR REPLACE FUNCTION pg_catalog.textlike(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.textne(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$textne$function$

CREATE OR REPLACE FUNCTION pg_catalog.textnlike(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textnlike$function$

CREATE OR REPLACE FUNCTION pg_catalog.textout(text)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textout$function$

CREATE OR REPLACE FUNCTION pg_catalog.textrecv(internal)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$textrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.textregexeq(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textregexeq$function$

CREATE OR REPLACE FUNCTION pg_catalog.textregexne(text, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$textregexne$function$

CREATE OR REPLACE FUNCTION pg_catalog.textsend(text)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$textsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.thesaurus_init(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$thesaurus_init$function$

CREATE OR REPLACE FUNCTION pg_catalog.thesaurus_lexize(internal, internal, internal, internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$thesaurus_lexize$function$

CREATE OR REPLACE FUNCTION pg_catalog.tideq(tid, tid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tideq$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidge(tid, tid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tidge$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidgt(tid, tid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tidgt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidin(cstring)
 RETURNS tid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tidin$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidlarger(tid, tid)
 RETURNS tid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tidlarger$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidle(tid, tid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tidle$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidlt(tid, tid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tidlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidne(tid, tid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tidne$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidout(tid)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tidout$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidrecv(internal)
 RETURNS tid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tidrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidsend(tid)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tidsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.tidsmaller(tid, tid)
 RETURNS tid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tidsmaller$function$

CREATE OR REPLACE FUNCTION pg_catalog."time"(time with time zone)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_time$function$

CREATE OR REPLACE FUNCTION pg_catalog."time"(timestamp with time zone)
 RETURNS time without time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_time$function$

CREATE OR REPLACE FUNCTION pg_catalog."time"(time without time zone, integer)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_scale$function$

CREATE OR REPLACE FUNCTION pg_catalog."time"(interval)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$interval_time$function$

CREATE OR REPLACE FUNCTION pg_catalog."time"(abstime)
 RETURNS time without time zone
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select cast(cast($1 as timestamp without time zone) as pg_catalog.time)$function$

CREATE OR REPLACE FUNCTION pg_catalog."time"(timestamp without time zone)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_cmp(time without time zone, time without time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_eq(time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$time_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_ge(time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$time_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_gt(time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$time_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_hash(time without time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_hash$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_in(cstring, oid, integer)
 RETURNS time without time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$time_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_larger(time without time zone, time without time zone)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_le(time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$time_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_lt(time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$time_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_mi_interval(time without time zone, interval)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_mi_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_mi_time(time without time zone, time without time zone)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_mi_time$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_ne(time without time zone, time without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$time_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_out(time without time zone)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_pl_interval(time without time zone, interval)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_pl_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_recv(internal, oid, integer)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_send(time without time zone)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_smaller(time without time zone, time without time zone)
 RETURNS time without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.time_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$time_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.timedate_pl(time without time zone, date)
 RETURNS timestamp without time zone
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select ($2 + $1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.timemi(abstime, reltime)
 RETURNS abstime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timemi$function$

CREATE OR REPLACE FUNCTION pg_catalog.timenow()
 RETURNS abstime
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timenow$function$

CREATE OR REPLACE FUNCTION pg_catalog.timeofday()
 RETURNS text
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$timeofday$function$

CREATE OR REPLACE FUNCTION pg_catalog.timepl(abstime, reltime)
 RETURNS abstime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timepl$function$

CREATE OR REPLACE FUNCTION pg_catalog."timestamp"(timestamp with time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog."timestamp"(date, time without time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datetime_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog."timestamp"(date)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$date_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog."timestamp"(abstime)
 RETURNS timestamp without time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$abstime_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog."timestamp"(timestamp without time zone, integer)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_scale$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_cmp(timestamp without time zone, timestamp without time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_cmp_date(timestamp without time zone, date)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_cmp_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_cmp_timestamptz(timestamp without time zone, timestamp with time zone)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_cmp_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_eq(timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_eq_date(timestamp without time zone, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_eq_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_eq_timestamptz(timestamp without time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_eq_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_ge(timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_ge_date(timestamp without time zone, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_ge_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_ge_timestamptz(timestamp without time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_ge_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_gt(timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_gt_date(timestamp without time zone, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_gt_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_gt_timestamptz(timestamp without time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_gt_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_hash(timestamp without time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_hash$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_in(cstring, oid, integer)
 RETURNS timestamp without time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_izone_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_izone_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_larger(timestamp without time zone, timestamp without time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_le(timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_le_date(timestamp without time zone, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_le_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_le_timestamptz(timestamp without time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_le_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_lt(timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_lt_date(timestamp without time zone, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_lt_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_lt_timestamptz(timestamp without time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_lt_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_mi(timestamp without time zone, timestamp without time zone)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_mi_interval(timestamp without time zone, interval)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_mi_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_ne(timestamp without time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_ne_date(timestamp without time zone, date)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_ne_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_ne_timestamptz(timestamp without time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_ne_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_out(timestamp without time zone)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_pl_interval(timestamp without time zone, interval)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_pl_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_recv(internal, oid, integer)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_send(timestamp without time zone)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_smaller(timestamp without time zone, timestamp without time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamp_zone_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_zone_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz(timestamp without time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz(timestamp with time zone, integer)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptz_scale$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz(date, time with time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$datetimetz_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz(date, time without time zone)
 RETURNS timestamp with time zone
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT COST 1
AS $function$select cast(($1 + $2) as timestamp with time zone)$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz(date)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$date_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz(abstime)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$abstime_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_cmp(timestamp with time zone, timestamp with time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_cmp_date(timestamp with time zone, date)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_cmp_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_cmp_timestamp(timestamp with time zone, timestamp without time zone)
 RETURNS integer
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_cmp_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_eq(timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_eq_date(timestamp with time zone, date)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_eq_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_eq_timestamp(timestamp with time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_eq_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_ge(timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_ge_date(timestamp with time zone, date)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_ge_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_ge_timestamp(timestamp with time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_ge_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_gt(timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_gt_date(timestamp with time zone, date)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_gt_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_gt_timestamp(timestamp with time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_gt_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_in(cstring, oid, integer)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_larger(timestamp with time zone, timestamp with time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_le(timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_le_date(timestamp with time zone, date)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_le_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_le_timestamp(timestamp with time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_le_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_lt(timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_lt_date(timestamp with time zone, date)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_lt_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_lt_timestamp(timestamp with time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_lt_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_mi(timestamp with time zone, timestamp with time zone)
 RETURNS interval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_mi$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_mi_interval(timestamp with time zone, interval)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_mi_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_ne(timestamp with time zone, timestamp with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timestamp_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_ne_date(timestamp with time zone, date)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_ne_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_ne_timestamp(timestamp with time zone, timestamp without time zone)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_ne_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_out(timestamp with time zone)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_pl_interval(timestamp with time zone, interval)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_pl_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_recv(internal, oid, integer)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptz_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_send(timestamp with time zone)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptz_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptz_smaller(timestamp with time zone, timestamp with time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptztypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptztypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.timestamptztypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptztypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz(time without time zone)
 RETURNS time with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$time_timetz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz(time with time zone, integer)
 RETURNS time with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_scale$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz(timestamp with time zone)
 RETURNS time with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_timetz$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_cmp(time with time zone, time with time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_eq(time with time zone, time with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timetz_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_ge(time with time zone, time with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timetz_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_gt(time with time zone, time with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timetz_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_hash(time with time zone)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_hash$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_in(cstring, oid, integer)
 RETURNS time with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timetz_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_larger(time with time zone, time with time zone)
 RETURNS time with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_larger$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_le(time with time zone, time with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timetz_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_lt(time with time zone, time with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timetz_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_mi_interval(time with time zone, interval)
 RETURNS time with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_mi_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_ne(time with time zone, time with time zone)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$timetz_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_out(time with time zone)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_pl_interval(time with time zone, interval)
 RETURNS time with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_pl_interval$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_recv(internal, oid, integer)
 RETURNS time with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_send(time with time zone)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetz_smaller(time with time zone, time with time zone)
 RETURNS time with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_smaller$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetzdate_pl(time with time zone, date)
 RETURNS timestamp with time zone
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select ($2 + $1)$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetztypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetztypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.timetztypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetztypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.timezone(interval, timestamp without time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_izone$function$

CREATE OR REPLACE FUNCTION pg_catalog.timezone(text, timestamp without time zone)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamp_zone$function$

CREATE OR REPLACE FUNCTION pg_catalog.timezone(interval, time with time zone)
 RETURNS time with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timetz_izone$function$

CREATE OR REPLACE FUNCTION pg_catalog.timezone(text, time with time zone)
 RETURNS time with time zone
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$timetz_zone$function$

CREATE OR REPLACE FUNCTION pg_catalog.timezone(text, timestamp with time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptz_zone$function$

CREATE OR REPLACE FUNCTION pg_catalog.timezone(interval, timestamp with time zone)
 RETURNS timestamp without time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$timestamptz_izone$function$

CREATE OR REPLACE FUNCTION pg_catalog.tinterval(abstime, abstime)
 RETURNS tinterval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$mktinterval$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalct(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalct$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalend(tinterval)
 RETURNS abstime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalend$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervaleq(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervaleq$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalge(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervalge$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalgt(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervalgt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalin(cstring)
 RETURNS tinterval
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$tintervalin$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalle(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervalle$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalleneq(tinterval, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervalleneq$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervallenge(tinterval, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervallenge$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervallengt(tinterval, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervallengt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervallenle(tinterval, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervallenle$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervallenlt(tinterval, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervallenlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervallenne(tinterval, reltime)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervallenne$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervallt(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervallt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalne(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$tintervalne$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalout(tinterval)
 RETURNS cstring
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$tintervalout$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalov(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalov$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalrecv(internal)
 RETURNS tinterval
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalrel(tinterval)
 RETURNS reltime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalrel$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalsame(tinterval, tinterval)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalsame$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalsend(tinterval)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.tintervalstart(tinterval)
 RETURNS abstime
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tintervalstart$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_ascii(text, name)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$to_ascii_encname$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_ascii(text, integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$to_ascii_enc$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_ascii(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$to_ascii_default$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(timestamp without time zone, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamp_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(double precision, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$float8_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(real, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$float4_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(bigint, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$int8_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(integer, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$int4_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(numeric, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$numeric_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(timestamp with time zone, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$timestamptz_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_char(interval, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$interval_to_char$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_date(text, text)
 RETURNS date
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_date$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_hex(bigint)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$to_hex64$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_hex(integer)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$to_hex32$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_json(anyelement)
 RETURNS json
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_json$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_jsonb(anyelement)
 RETURNS jsonb
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_jsonb$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_number(text, text)
 RETURNS numeric
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$numeric_to_number$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regclass(text)
 RETURNS regclass
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regclass$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regnamespace(text)
 RETURNS regnamespace
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regnamespace$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regoper(text)
 RETURNS regoper
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regoper$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regoperator(text)
 RETURNS regoperator
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regoperator$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regproc(text)
 RETURNS regproc
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regproc$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regprocedure(text)
 RETURNS regprocedure
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regprocedure$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regrole(text)
 RETURNS regrole
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regrole$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_regtype(text)
 RETURNS regtype
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_regtype$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_timestamp(text, text)
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$to_timestamp$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_timestamp(double precision)
 RETURNS timestamp with time zone
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$float8_timestamptz$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_tsquery(text)
 RETURNS tsquery
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$to_tsquery$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_tsquery(regconfig, text)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT COST 100
AS $function$to_tsquery_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_tsvector(text)
 RETURNS tsvector
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$to_tsvector$function$

CREATE OR REPLACE FUNCTION pg_catalog.to_tsvector(regconfig, text)
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT COST 100
AS $function$to_tsvector_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.transaction_timestamp()
 RETURNS timestamp with time zone
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$now$function$

CREATE OR REPLACE FUNCTION pg_catalog.translate(text, text, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$translate$function$

CREATE OR REPLACE FUNCTION pg_catalog.trigger_in(cstring)
 RETURNS trigger
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$trigger_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.trigger_out(trigger)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$trigger_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.trunc(macaddr)
 RETURNS macaddr
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$macaddr_trunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.trunc(numeric)
 RETURNS numeric
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.trunc($1,0)$function$

CREATE OR REPLACE FUNCTION pg_catalog.trunc(numeric, integer)
 RETURNS numeric
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$numeric_trunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.trunc(double precision)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$dtrunc$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_debug(document text, OUT alias text, OUT description text, OUT token text, OUT dictionaries regdictionary[], OUT dictionary regdictionary, OUT lexemes text[])
 RETURNS SETOF record
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT
AS $function$
    SELECT * FROM pg_catalog.ts_debug( pg_catalog.get_current_ts_config(), $1);
$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_debug(config regconfig, document text, OUT alias text, OUT description text, OUT token text, OUT dictionaries regdictionary[], OUT dictionary regdictionary, OUT lexemes text[])
 RETURNS SETOF record
 LANGUAGE sql
 STABLE PARALLEL SAFE STRICT
AS $function$
SELECT
    tt.alias AS alias,
    tt.description AS description,
    parse.token AS token,
    ARRAY ( SELECT m.mapdict::pg_catalog.regdictionary
            FROM pg_catalog.pg_ts_config_map AS m
            WHERE m.mapcfg = $1 AND m.maptokentype = parse.tokid
            ORDER BY m.mapseqno )
    AS dictionaries,
    ( SELECT mapdict::pg_catalog.regdictionary
      FROM pg_catalog.pg_ts_config_map AS m
      WHERE m.mapcfg = $1 AND m.maptokentype = parse.tokid
      ORDER BY pg_catalog.ts_lexize(mapdict, parse.token) IS NULL, m.mapseqno
      LIMIT 1
    ) AS dictionary,
    ( SELECT pg_catalog.ts_lexize(mapdict, parse.token)
      FROM pg_catalog.pg_ts_config_map AS m
      WHERE m.mapcfg = $1 AND m.maptokentype = parse.tokid
      ORDER BY pg_catalog.ts_lexize(mapdict, parse.token) IS NULL, m.mapseqno
      LIMIT 1
    ) AS lexemes
FROM pg_catalog.ts_parse(
        (SELECT cfgparser FROM pg_catalog.pg_ts_config WHERE oid = $1 ), $2
    ) AS parse,
     pg_catalog.ts_token_type(
        (SELECT cfgparser FROM pg_catalog.pg_ts_config WHERE oid = $1 )
    ) AS tt
WHERE tt.tokid = parse.tokid
$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_delete(tsvector, text[])
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_delete_arr$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_delete(tsvector, text)
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_delete_str$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_filter(tsvector, "char"[])
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_filter$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_headline(text, tsquery)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$ts_headline$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_headline(text, tsquery, text)
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$ts_headline_opt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_headline(regconfig, text, tsquery)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT COST 100
AS $function$ts_headline_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_headline(regconfig, text, tsquery, text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT COST 100
AS $function$ts_headline_byid_opt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_lexize(regdictionary, text)
 RETURNS text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_lexize$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_match_qv(tsquery, tsvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_match_qv$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_match_tq(text, tsquery)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$ts_match_tq$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_match_tt(text, text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT COST 100
AS $function$ts_match_tt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_match_vq(tsvector, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_match_vq$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_parse(parser_name text, txt text, OUT tokid integer, OUT token text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$ts_parse_byname$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_parse(parser_oid oid, txt text, OUT tokid integer, OUT token text)
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_parse_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank(tsvector, tsquery)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rank_tt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank(tsvector, tsquery, integer)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rank_ttf$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank(real[], tsvector, tsquery)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rank_wtt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank(real[], tsvector, tsquery, integer)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rank_wttf$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank_cd(tsvector, tsquery)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rankcd_tt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank_cd(tsvector, tsquery, integer)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rankcd_ttf$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank_cd(real[], tsvector, tsquery)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rankcd_wtt$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rank_cd(real[], tsvector, tsquery, integer)
 RETURNS real
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$ts_rankcd_wttf$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rewrite(tsquery, text)
 RETURNS tsquery
 LANGUAGE internal
 PARALLEL SAFE STRICT COST 100
AS $function$tsquery_rewrite_query$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_rewrite(tsquery, tsquery, tsquery)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_rewrite$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_stat(query text, weights text, OUT word text, OUT ndoc integer, OUT nentry integer)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL SAFE STRICT COST 10 ROWS 10000
AS $function$ts_stat2$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_stat(query text, OUT word text, OUT ndoc integer, OUT nentry integer)
 RETURNS SETOF record
 LANGUAGE internal
 PARALLEL SAFE STRICT COST 10 ROWS 10000
AS $function$ts_stat1$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_token_type(parser_name text, OUT tokid integer, OUT alias text, OUT description text)
 RETURNS SETOF record
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT ROWS 16
AS $function$ts_token_type_byname$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_token_type(parser_oid oid, OUT tokid integer, OUT alias text, OUT description text)
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 16
AS $function$ts_token_type_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.ts_typanalyze(internal)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$ts_typanalyze$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsm_handler_in(cstring)
 RETURNS tsm_handler
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$tsm_handler_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsm_handler_out(tsm_handler)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsm_handler_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsmatchjoinsel(internal, oid, internal, smallint, internal)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$tsmatchjoinsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsmatchsel(internal, oid, internal, integer)
 RETURNS double precision
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$tsmatchsel$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsq_mcontained(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsq_mcontained$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsq_mcontains(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsq_mcontains$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_and(tsquery, tsquery)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_and$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_cmp(tsquery, tsquery)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_eq(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_ge(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_gt(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_le(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_lt(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_ne(tsquery, tsquery)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_not(tsquery)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_not$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_or(tsquery, tsquery)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_or$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_phrase(tsquery, tsquery, integer)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_phrase_distance$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquery_phrase(tsquery, tsquery)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquery_phrase$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsqueryin(cstring)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsqueryin$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsqueryout(tsquery)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsqueryout$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsqueryrecv(internal)
 RETURNS tsquery
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsqueryrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsquerysend(tsquery)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsquerysend$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsrange(timestamp without time zone, timestamp without time zone, text)
 RETURNS tsrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor3$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsrange(timestamp without time zone, timestamp without time zone)
 RETURNS tsrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor2$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsrange_subdiff(timestamp without time zone, timestamp without time zone)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsrange_subdiff$function$

CREATE OR REPLACE FUNCTION pg_catalog.tstzrange(timestamp with time zone, timestamp with time zone, text)
 RETURNS tstzrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor3$function$

CREATE OR REPLACE FUNCTION pg_catalog.tstzrange(timestamp with time zone, timestamp with time zone)
 RETURNS tstzrange
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$range_constructor2$function$

CREATE OR REPLACE FUNCTION pg_catalog.tstzrange_subdiff(timestamp with time zone, timestamp with time zone)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tstzrange_subdiff$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_cmp(tsvector, tsvector)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_concat(tsvector, tsvector)
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_concat$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_eq(tsvector, tsvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_ge(tsvector, tsvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_gt(tsvector, tsvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_le(tsvector, tsvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_lt(tsvector, tsvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_ne(tsvector, tsvector)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_to_array(tsvector)
 RETURNS text[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvector_to_array$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_update_trigger()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE
AS $function$tsvector_update_trigger_byid$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvector_update_trigger_column()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE
AS $function$tsvector_update_trigger_bycolumn$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvectorin(cstring)
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvectorin$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvectorout(tsvector)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvectorout$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvectorrecv(internal)
 RETURNS tsvector
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvectorrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.tsvectorsend(tsvector)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$tsvectorsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_current()
 RETURNS bigint
 LANGUAGE internal
 STABLE STRICT
AS $function$txid_current$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_current_snapshot()
 RETURNS txid_snapshot
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$txid_current_snapshot$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_snapshot_in(cstring)
 RETURNS txid_snapshot
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$txid_snapshot_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_snapshot_out(txid_snapshot)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$txid_snapshot_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_snapshot_recv(internal)
 RETURNS txid_snapshot
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$txid_snapshot_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_snapshot_send(txid_snapshot)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$txid_snapshot_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_snapshot_xip(txid_snapshot)
 RETURNS SETOF bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 50
AS $function$txid_snapshot_xip$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_snapshot_xmax(txid_snapshot)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$txid_snapshot_xmax$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_snapshot_xmin(txid_snapshot)
 RETURNS bigint
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$txid_snapshot_xmin$function$

CREATE OR REPLACE FUNCTION pg_catalog.txid_visible_in_snapshot(bigint, txid_snapshot)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$txid_visible_in_snapshot$function$

CREATE OR REPLACE FUNCTION pg_catalog.uhc_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_uhc', $function$uhc_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.unique_key_recheck()
 RETURNS trigger
 LANGUAGE internal
 PARALLEL SAFE STRICT
AS $function$unique_key_recheck$function$

CREATE OR REPLACE FUNCTION pg_catalog.unknownin(cstring)
 RETURNS unknown
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$unknownin$function$

CREATE OR REPLACE FUNCTION pg_catalog.unknownout(unknown)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$unknownout$function$

CREATE OR REPLACE FUNCTION pg_catalog.unknownrecv(internal)
 RETURNS unknown
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$unknownrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.unknownsend(unknown)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$unknownsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.unnest(tsvector tsvector, OUT lexeme text, OUT positions smallint[], OUT weights text[])
 RETURNS SETOF record
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 10
AS $function$tsvector_unnest$function$

CREATE OR REPLACE FUNCTION pg_catalog.unnest(anyarray)
 RETURNS SETOF anyelement
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT ROWS 100
AS $function$array_unnest$function$

CREATE OR REPLACE FUNCTION pg_catalog.upper(text)
 RETURNS text
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$upper$function$

CREATE OR REPLACE FUNCTION pg_catalog.upper(anyrange)
 RETURNS anyelement
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_upper$function$

CREATE OR REPLACE FUNCTION pg_catalog.upper_inc(anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_upper_inc$function$

CREATE OR REPLACE FUNCTION pg_catalog.upper_inf(anyrange)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$range_upper_inf$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_ascii(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_ascii', $function$utf8_to_ascii$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_big5(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_big5', $function$utf8_to_big5$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_euc_cn(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_cn', $function$utf8_to_euc_cn$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_euc_jis_2004(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc2004', $function$utf8_to_euc_jis_2004$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_euc_jp(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_jp', $function$utf8_to_euc_jp$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_euc_kr(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_kr', $function$utf8_to_euc_kr$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_euc_tw(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_euc_tw', $function$utf8_to_euc_tw$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_gb18030(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_gb18030', $function$utf8_to_gb18030$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_gbk(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_gbk', $function$utf8_to_gbk$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_iso8859(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_iso8859', $function$utf8_to_iso8859$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_iso8859_1(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_iso8859_1', $function$utf8_to_iso8859_1$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_johab(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_johab', $function$utf8_to_johab$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_koi8r(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_cyrillic', $function$utf8_to_koi8r$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_koi8u(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_cyrillic', $function$utf8_to_koi8u$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_shift_jis_2004(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_sjis2004', $function$utf8_to_shift_jis_2004$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_sjis(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_sjis', $function$utf8_to_sjis$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_uhc(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_uhc', $function$utf8_to_uhc$function$

CREATE OR REPLACE FUNCTION pg_catalog.utf8_to_win(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_win', $function$utf8_to_win$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_cmp(uuid, uuid)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$uuid_cmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_eq(uuid, uuid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$uuid_eq$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_ge(uuid, uuid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$uuid_ge$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_gt(uuid, uuid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$uuid_gt$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_hash(uuid)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$uuid_hash$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_in(cstring)
 RETURNS uuid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$uuid_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_le(uuid, uuid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$uuid_le$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_lt(uuid, uuid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$uuid_lt$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_ne(uuid, uuid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$uuid_ne$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_out(uuid)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$uuid_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_recv(internal)
 RETURNS uuid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$uuid_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_send(uuid)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$uuid_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.uuid_sortsupport(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$uuid_sortsupport$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbit(bit varying, integer, boolean)
 RETURNS bit varying
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbit$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbit_in(cstring, oid, integer)
 RETURNS bit varying
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbit_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbit_out(bit varying)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbit_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbit_recv(internal, oid, integer)
 RETURNS bit varying
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbit_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbit_send(bit varying)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbit_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbit_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbit_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbitcmp(bit varying, bit varying)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$bitcmp$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbiteq(bit varying, bit varying)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$biteq$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbitge(bit varying, bit varying)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitge$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbitgt(bit varying, bit varying)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitgt$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbitle(bit varying, bit varying)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitle$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbitlt(bit varying, bit varying)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitlt$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbitne(bit varying, bit varying)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$bitne$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbittypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbittypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.varbittypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varbittypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog."varchar"(character varying, integer, boolean)
 RETURNS character varying
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varchar$function$

CREATE OR REPLACE FUNCTION pg_catalog."varchar"(name)
 RETURNS character varying
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$name_text$function$

CREATE OR REPLACE FUNCTION pg_catalog.varchar_transform(internal)
 RETURNS internal
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varchar_transform$function$

CREATE OR REPLACE FUNCTION pg_catalog.varcharin(cstring, oid, integer)
 RETURNS character varying
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varcharin$function$

CREATE OR REPLACE FUNCTION pg_catalog.varcharout(character varying)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varcharout$function$

CREATE OR REPLACE FUNCTION pg_catalog.varcharrecv(internal, oid, integer)
 RETURNS character varying
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$varcharrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.varcharsend(character varying)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$varcharsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.varchartypmodin(cstring[])
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varchartypmodin$function$

CREATE OR REPLACE FUNCTION pg_catalog.varchartypmodout(integer)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$varchartypmodout$function$

CREATE OR REPLACE FUNCTION pg_catalog.version()
 RETURNS text
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$pgsql_version$function$

CREATE OR REPLACE FUNCTION pg_catalog.void_in(cstring)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$void_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.void_out(void)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$void_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.void_recv(internal)
 RETURNS void
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$void_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.void_send(void)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$void_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.width(box)
 RETURNS double precision
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$box_width$function$

CREATE OR REPLACE FUNCTION pg_catalog.width_bucket(anyelement, anyarray)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$width_bucket_array$function$

CREATE OR REPLACE FUNCTION pg_catalog.width_bucket(double precision, double precision, double precision, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$width_bucket_float8$function$

CREATE OR REPLACE FUNCTION pg_catalog.width_bucket(numeric, numeric, numeric, integer)
 RETURNS integer
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$width_bucket_numeric$function$

CREATE OR REPLACE FUNCTION pg_catalog.win1250_to_latin2(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin2_and_win1250', $function$win1250_to_latin2$function$

CREATE OR REPLACE FUNCTION pg_catalog.win1250_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/latin2_and_win1250', $function$win1250_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.win1251_to_iso(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win1251_to_iso$function$

CREATE OR REPLACE FUNCTION pg_catalog.win1251_to_koi8r(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win1251_to_koi8r$function$

CREATE OR REPLACE FUNCTION pg_catalog.win1251_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win1251_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.win1251_to_win866(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win1251_to_win866$function$

CREATE OR REPLACE FUNCTION pg_catalog.win866_to_iso(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win866_to_iso$function$

CREATE OR REPLACE FUNCTION pg_catalog.win866_to_koi8r(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win866_to_koi8r$function$

CREATE OR REPLACE FUNCTION pg_catalog.win866_to_mic(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win866_to_mic$function$

CREATE OR REPLACE FUNCTION pg_catalog.win866_to_win1251(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/cyrillic_and_mic', $function$win866_to_win1251$function$

CREATE OR REPLACE FUNCTION pg_catalog.win_to_utf8(integer, integer, cstring, internal, integer)
 RETURNS void
 LANGUAGE c
 PARALLEL SAFE STRICT
AS '$libdir/utf8_and_win', $function$win_to_utf8$function$

CREATE OR REPLACE FUNCTION pg_catalog.xideq(xid, xid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$xideq$function$

CREATE OR REPLACE FUNCTION pg_catalog.xideqint4(xid, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$xideq$function$

CREATE OR REPLACE FUNCTION pg_catalog.xidin(cstring)
 RETURNS xid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xidin$function$

CREATE OR REPLACE FUNCTION pg_catalog.xidneq(xid, xid)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$xidneq$function$

CREATE OR REPLACE FUNCTION pg_catalog.xidneqint4(xid, integer)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT LEAKPROOF
AS $function$xidneq$function$

CREATE OR REPLACE FUNCTION pg_catalog.xidout(xid)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xidout$function$

CREATE OR REPLACE FUNCTION pg_catalog.xidrecv(internal)
 RETURNS xid
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xidrecv$function$

CREATE OR REPLACE FUNCTION pg_catalog.xidsend(xid)
 RETURNS bytea
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xidsend$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml(text)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$texttoxml$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml_in(cstring)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$xml_in$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml_is_well_formed(text)
 RETURNS boolean
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$xml_is_well_formed$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml_is_well_formed_content(text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xml_is_well_formed_content$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml_is_well_formed_document(text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xml_is_well_formed_document$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml_out(xml)
 RETURNS cstring
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xml_out$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml_recv(internal)
 RETURNS xml
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$xml_recv$function$

CREATE OR REPLACE FUNCTION pg_catalog.xml_send(xml)
 RETURNS bytea
 LANGUAGE internal
 STABLE PARALLEL SAFE STRICT
AS $function$xml_send$function$

CREATE OR REPLACE FUNCTION pg_catalog.xmlcomment(text)
 RETURNS xml
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xmlcomment$function$

CREATE OR REPLACE FUNCTION pg_catalog.xmlconcat2(xml, xml)
 RETURNS xml
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE
AS $function$xmlconcat2$function$

CREATE OR REPLACE FUNCTION pg_catalog."xmlexists"(text, xml)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xmlexists$function$

CREATE OR REPLACE FUNCTION pg_catalog.xmlvalidate(xml, text)
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xmlvalidate$function$

CREATE OR REPLACE FUNCTION pg_catalog.xpath(text, xml)
 RETURNS xml[]
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.xpath($1, $2, '{}'::pg_catalog.text[])$function$

CREATE OR REPLACE FUNCTION pg_catalog.xpath(text, xml, text[])
 RETURNS xml[]
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xpath$function$

CREATE OR REPLACE FUNCTION pg_catalog.xpath_exists(text, xml)
 RETURNS boolean
 LANGUAGE sql
 IMMUTABLE PARALLEL SAFE STRICT COST 1
AS $function$select pg_catalog.xpath_exists($1, $2, '{}'::pg_catalog.text[])$function$

CREATE OR REPLACE FUNCTION pg_catalog.xpath_exists(text, xml, text[])
 RETURNS boolean
 LANGUAGE internal
 IMMUTABLE PARALLEL SAFE STRICT
AS $function$xpath_exists$function$

