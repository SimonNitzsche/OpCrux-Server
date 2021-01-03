USE [OPCRUX_GD]
GO

CREATE TABLE [dbo].[Pets](
	[id] [int] NOT NULL,
	[ownerID] [bigint] NOT NULL,
	[petName] [varchar](50) NOT NULL,
	[petTemplate] [int] NOT NULL
) ON [PRIMARY]
GO

USE [OPCRUX_CD]
GO

INSERT INTO [dbo].[IDCounter] 
VALUES ('PETS', 0);
GO