# OpCrux Server

## About

OpCrux is a work in progress LEGO Universe server emulator.

LEGO Universe was an MMORPG made by NetDevil and The LEGO Group, run from 2010-2012. Since it's closure, fans have
reverse engineered the game to develop servers for the LEGO Universe client.

For more information, see the [LU Server Projects website](https://lusprojects.github.io/).

## Setup

### Prerequisites

#### Python
[Python 3](https://www.python.org/) is required for some utility scripts

#### Client Files
Some resources from the LEGO Universe client are needed. An easy way to get them is by downloading an unpacked client. The contents of the `res`
folder of the client should be placed in `bin/res`. Note that not all files are needed - see `bin/res/include_global.txt`
for a list of required files.

#### Database
The database uses ODBC (or unixODBC on Unix systems) to connect to any DB that has an ODBC connector. However, our SQL
files are written in TSQL (Transact SQL), so MSSQL would be the easiest DB to use (though you can convert the SQL file
to be used with any other DB such as Postgres, MySQL etc).

Some useful references:
- [SQL Server installation guide](https://docs.microsoft.com/en-us/sql/database-engine/install-windows/install-sql-server?view=sql-server-ver15)
- [SQL Server setup guides for Linux](https://docs.microsoft.com/en-us/sql/linux/sql-server-linux-overview?view=sql-server-ver15)
- [SQL Server technical documentation](https://docs.microsoft.com/en-us/sql/sql-server/?view=sql-server-ver15)
- [How To Set up a Microsoft SQL Server ODBC Data Source](https://support.microsoft.com/en-us/help/965049/how-to-set-up-a-microsoft-sql-server-odbc-data-source)

### Building
Building OpCrux is handled using a [CMake](https://cmake.org/) toolchain, which can be invoked from your IDE or CLI.

#### Linux CLI
```sh
cmake . -B ./build && cmake --build ./build
cd bin
./OpCrux_Server
```

### Configuration
The first time you run the `OpCrux_Server` binary, configuration files will be generated in a `conf` folder in the
current working directory (which should be `bin`). You may want to change some of these settings for your convenience,
but you must at least fill out the DBConnection section in `bin/conf/Database.ini` (note - DBDRIVER is the name of the
ODBC driver you want to use, or the file path to its library on your system).

### Database Initialization
To set up the necessary schemas and tables, run the migration files via the python script `extra/database/migrate.py`

You'll also most likely want to make a user account as well. In the future there will be a proper tool for this,
but for now you can run the following:
```sql
INSERT INTO OPCRUX_AD.dbo.Accounts (username, email, password, rank)
VALUES ("<username>", "<email>", "<SHA512 hashed password>", 9);
```

You can also use bcrypt by using the AccountService contained in /services/CruxAuthService

## Community
We have a public discord server with a development channel [here](http://discord.opcrux.org)

Please note that we only offer setup help to prospective developers.

## Disclaimer
The LEGO Group has not endorsed or authorized the operation of this game and is not liable for any safety issues in relation to its operation.
