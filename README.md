# OpCrux Server

## Disclaimer
> The LEGO Group has not endorsed or authorized the operation of this game and is not liable for any safety issues in relation to its operation.

## What is this

This is a WIP (Work in progress) LEGO Universe server.

LEGO Universe was a game made by NetDevil and LEGO released in October 2010 which had the servers closed later in 2012 due to moderation costs being too high but since then ex-players have reverse engineered the game to develop servers for the LEGO Universe client. 

## Discord

We have a public discord server with a development channel [here](http://discord.opcrux.org)

## Setup

We have a CMake build system so compilation on Windows and Linux should be as simple using your IDE or a CLI

The database uses ODBC (or unixODBC on Unix systems) to connect to any DB that would have a connector for ODBC but we have our SQL files in TSQL (Transact SQL) so MSSQL would be the easiest DB to use but you can convert the SQL file to be used with any other DB (Postgres, MySQL etc).

### Some Useful resources for Setting up the SQL Server and ODBC:
[SQL Server installation guide](https://docs.microsoft.com/en-us/sql/database-engine/install-windows/install-sql-server?view=sql-server-ver15)

[SQL Server technical documentation](https://docs.microsoft.com/en-us/sql/sql-server/?view=sql-server-ver15)

[How To Set up a Microsoft SQL Server ODBC Data Source](https://support.microsoft.com/en-us/help/965049/how-to-set-up-a-microsoft-sql-server-odbc-data-source)

(We only offer help setting up the server if you plan to help development)
