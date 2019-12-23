/*
   Montag, 23. Dezember 201915:38:01
   Benutzer: 
   Server: localhost
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
CREATE TABLE dbo.Tmp_FlagChunks
	(
	playerID bigint NOT NULL,
	chunkID int NOT NULL,
	chunkData bigint NOT NULL
	)  ON [PRIMARY]
GO
ALTER TABLE dbo.Tmp_FlagChunks SET (LOCK_ESCALATION = TABLE)
GO
IF EXISTS(SELECT * FROM dbo.FlagChunks)
	 EXEC('INSERT INTO dbo.Tmp_FlagChunks (playerID, chunkID, chunkData)
		SELECT playerID, chunkID, chunkData FROM dbo.FlagChunks WITH (HOLDLOCK TABLOCKX)')
GO
DROP TABLE dbo.FlagChunks
GO
EXECUTE sp_rename N'dbo.Tmp_FlagChunks', N'FlagChunks', 'OBJECT' 
GO
COMMIT
select Has_Perms_By_Name(N'dbo.FlagChunks', 'Object', 'ALTER') as ALT_Per, Has_Perms_By_Name(N'dbo.FlagChunks', 'Object', 'VIEW DEFINITION') as View_def_Per, Has_Perms_By_Name(N'dbo.FlagChunks', 'Object', 'CONTROL') as Contr_Per 