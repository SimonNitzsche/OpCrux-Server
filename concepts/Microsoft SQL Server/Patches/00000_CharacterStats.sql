/*
   Samstag, 17. August 201902:07:11
   Benutzer: 
   Server: DESKTOP-U26DIDP
   Datenbank: OPCRUX_GD
   Anwendung: 
*/

/* Überprüfen Sie das Skript gründlich, bevor Sie es außerhalb des Datenbank-Designer-Kontexts ausführen, um potenzielle Datenverluste zu vermeiden.*/
BEGIN TRANSACTION
SET QUOTED_IDENTIFIER ON
SET ARITHABORT ON
SET NUMERIC_ROUNDABORT OFF
SET CONCAT_NULL_YIELDS_NULL ON
SET ANSI_NULLS ON
SET ANSI_PADDING ON
SET ANSI_WARNINGS ON
COMMIT
BEGIN TRANSACTION
GO
CREATE TABLE dbo.CharacterStats
	(
	playerID bigint NOT NULL,
	currencyCollected bigint NOT NULL,
	bricksCollected bigint NOT NULL,
	objectsSmashed bigint NOT NULL,
	quickbuildsCompleted bigint NOT NULL,
	enemiesSmashed bigint NOT NULL,
	rocketsUsed bigint NOT NULL,
	missionsCompleted bigint NOT NULL,
	petsTamed bigint NOT NULL,
	imaginationCollected bigint NOT NULL,
	healthCollected bigint NOT NULL,
	armorCollected bigint NOT NULL,
	distanceTraveled bigint NOT NULL,
	smashed bigint NOT NULL,
	damageTaken bigint NOT NULL,
	damageHealed bigint NOT NULL,
	armorRepaired bigint NOT NULL,
	imaginationRestored bigint NOT NULL,
	imaginationUsed bigint NOT NULL,
	distanceDriven bigint NOT NULL,
	airborneTime bigint NOT NULL,
	racingImaginationCollected bigint NOT NULL,
	racingImaginationCratesSmashed bigint NOT NULL,
	racingBoostsActivated bigint NOT NULL,
	racingWrecks bigint NOT NULL,
	racingSmashablesSmashed bigint NOT NULL,
	racesFinished bigint NOT NULL,
	racesWon bigint NOT NULL
	)  ON [PRIMARY]
GO
ALTER TABLE dbo.CharacterStats SET (LOCK_ESCALATION = TABLE)
GO
COMMIT
select Has_Perms_By_Name(N'dbo.CharacterStats', 'Object', 'ALTER') as ALT_Per, Has_Perms_By_Name(N'dbo.CharacterStats', 'Object', 'VIEW DEFINITION') as View_def_Per, Has_Perms_By_Name(N'dbo.CharacterStats', 'Object', 'CONTROL') as Contr_Per 