/*
   Samstag, 17. August 201902:10:00
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
EXECUTE sp_rename N'dbo.CharacterStats.playerID', N'Tmp_statsID', 'COLUMN' 
GO
EXECUTE sp_rename N'dbo.CharacterStats.Tmp_statsID', N'statsID', 'COLUMN' 
GO
ALTER TABLE dbo.CharacterStats SET (LOCK_ESCALATION = TABLE)
GO
COMMIT
select Has_Perms_By_Name(N'dbo.CharacterStats', 'Object', 'ALTER') as ALT_Per, Has_Perms_By_Name(N'dbo.CharacterStats', 'Object', 'VIEW DEFINITION') as View_def_Per, Has_Perms_By_Name(N'dbo.CharacterStats', 'Object', 'CONTROL') as Contr_Per 