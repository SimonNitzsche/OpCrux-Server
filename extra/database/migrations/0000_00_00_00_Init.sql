CREATE DATABASE OPCRUX_AD;
CREATE DATABASE OPCRUX_CD;
CREATE DATABASE OPCRUX_GD;
GO

USE OPCRUX_GD;

CREATE TABLE [dbo].[Characters](
	[objectID] [bigint] IDENTITY(1,1) NOT NULL,
	[accountID] [int] NOT NULL,
	[charIndex] [tinyint] NOT NULL,
	[name] [varchar](50) NOT NULL,
	[pendingName] [varchar](50) NULL,
	[styleID] [int] NOT NULL,
	[statsID] [int] NOT NULL,
	[lastWorld] [smallint] NOT NULL,
	[lastInstance] [smallint] NOT NULL,
	[lastClone] [int] NOT NULL,
	[lastLog] [bigint] NOT NULL,
	[positionX] [float] NOT NULL,
	[positionY] [float] NOT NULL,
	[positionZ] [float] NOT NULL,
	[shirtObjectID] [bigint] NOT NULL,
	[pantsObjectID] [bigint] NOT NULL,
	[uScore] [int] NOT NULL,
	[uLevel] [int] NOT NULL,
	[currency] [int] NOT NULL,
	[reputation] [int] NOT NULL,
	[health] [int] NOT NULL,
	[imagination] [int] NOT NULL,
	[armor] [int] NOT NULL
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[CharacterStyles](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[headColor] [int] NOT NULL,
	[head] [int] NOT NULL,
	[chestColor] [int] NOT NULL,
	[chest] [int] NOT NULL,
	[legs] [int] NOT NULL,
	[hairStyle] [int] NOT NULL,
	[hairColor] [int] NOT NULL,
	[leftHand] [int] NOT NULL,
	[rightHand] [int] NOT NULL,
	[eyebrowStyle] [int] NOT NULL,
	[eyesStyle] [int] NOT NULL,
	[mouthStyle] [int] NOT NULL,
) ON [PRIMARY]
GO
CREATE TABLE [dbo].[CharacterStats](
	[statsID] [int] NOT NULL,
	[TotalCurrencyCollected] [int] NOT NULL,
	[TotalBricksCollected] [int] NOT NULL,
	[TotalSmashablesSmashed] [int] NOT NULL,
	[TotalQuickBuildsCompleted] [int] NOT NULL,
	[TotalEnemiesSmashed] [int] NOT NULL,
	[TotalRocketsUsed] [int] NOT NULL,
	[TotalPetsTamed] [int] NOT NULL,
	[TotalImaginationPowerUpsCollected] [int] NOT NULL,
	[TotalLifePowerUpsCollected] [int] NOT NULL,
	[TotalArmorPowerUpsCollected] [int] NOT NULL,
	[TotalDistanceTraveled] [int] NOT NULL,
	[TotalSuicides] [int] NOT NULL,
	[TotalDamageTaken] [int] NOT NULL,
	[TotalDamageHealed] [int] NOT NULL,
	[TotalArmorRepaired] [int] NOT NULL,
	[TotalImaginationRestored] [int] NOT NULL,
	[TotalImaginationUsed] [int] NOT NULL,
	[TotalDistanceDriven] [int] NOT NULL,
	[TotalTimeAirborne] [int] NOT NULL,
	[TotalRacingImaginationPowerUpsCollected] [int] NOT NULL,
	[TotalRacecarBoostsActivated] [int] NOT NULL,
	[TotalRacecarWrecks] [int] NOT NULL,
	[TotalRacingSmashablesSmashed] [int] NOT NULL,
	[TotalRacesFinished] [int] NOT NULL,
	[TotalFirstPlaceFinishes] [int] NOT NULL
) ON [PRIMARY]
GO
CREATE TABLE [dbo].[Inventory](
	[objectID] [bigint] NOT NULL,
	[ownerID] [bigint] NOT NULL,
	[subkey] [bigint] NULL,
	[tab] [int] NOT NULL,
	[slot] [int] NOT NULL,
	[template] [int] NOT NULL,
	[count] [int] NOT NULL,
	[attributes] [smallint] NULL,
	[metadata] [text] NULL
) ON [PRIMARY]
TEXTIMAGE_ON [PRIMARY]
GO
CREATE TABLE [dbo].[FlagChunks] (
	[playerID] [bigint] NOT NULL,
	[chunkID] [int] NOT NULL,
	[chunkData] [bigint] NOT NULL
) ON [PRIMARY]
GO
CREATE TABLE [dbo].[Missions] (
	[charID] [bigint],
	[missionID] [int],
	[state] [int],
	[progress] [varchar](128),
	[repeatCount] [int],
	[time] [bigint],
	[chosenReward] [int]
) ON [PRIMARY]
GO
CREATE TABLE [dbo].[UnlockedEmotes] (
	[playerID] [bigint],
	[emoteID] [int]
) ON [PRIMARY]
GO

USE OPCRUX_AD;
CREATE TABLE [dbo].[Accounts](
	[id] [int] IDENTITY(0,1) NOT NULL,
	[username] [varchar](33) NOT NULL,
	[password] [char](128) NOT NULL,
	[rank] [int] NULL,
	[email] [varchar] (128)
) ON [PRIMARY]
GO

USE OPCRUX_CD;

CREATE TABLE [dbo].[IDCounter](
	[type] [varchar](50) NOT NULL,
	[counter] [bigint] NOT NULL
) ON [PRIMARY]
GO

INSERT INTO dbo.IDCounter VALUES ('STATIC', 0)
INSERT INTO dbo.IDCounter VALUES ('PLAYER', 0)
INSERT INTO dbo.IDCounter VALUES ('P_STYLE', 0)
INSERT INTO dbo.IDCounter VALUES ('P_STATS', 0)
