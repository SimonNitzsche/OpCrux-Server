USE OPCRUX_GD;

CREATE TABLE dbo.Mail (
	mailID bigint NOT NULL IDENTITY (1, 1),
	receiver bigint NOT NULL,
	subject varchar(50) NOT NULL,
	body varchar(400) NOT NULL,
	sender varchar(32) NOT NULL,
	attachedCurrency bigint NOT NULL,
	attachedObjectID bigint NOT NULL,
	attachedLOT int NOT NULL,
	attachedSubkey bigint NOT NULL,
	expirationDate bigint NOT NULL,
	sendDate bigint NOT NULL,
	markedAsSeen bit NOT NULL
)
GO
