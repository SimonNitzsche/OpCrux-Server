USE [master]
GO
/****** Object:  Database [OPCRUX_AD]    Script Date: 20.12.2019 16:13:55 ******/
CREATE DATABASE [OPCRUX_AD]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'OPCRUX_AD', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER\MSSQL\DATA\OPCRUX_AD.mdf' , SIZE = 8192KB , MAXSIZE = UNLIMITED, FILEGROWTH = 65536KB )
 LOG ON 
( NAME = N'OPCRUX_AD_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER\MSSQL\DATA\OPCRUX_AD_log.ldf' , SIZE = 8192KB , MAXSIZE = 2048GB , FILEGROWTH = 65536KB )
GO
ALTER DATABASE [OPCRUX_AD] SET COMPATIBILITY_LEVEL = 140
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [OPCRUX_AD].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [OPCRUX_AD] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET ARITHABORT OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [OPCRUX_AD] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [OPCRUX_AD] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET  DISABLE_BROKER 
GO
ALTER DATABASE [OPCRUX_AD] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [OPCRUX_AD] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET RECOVERY FULL 
GO
ALTER DATABASE [OPCRUX_AD] SET  MULTI_USER 
GO
ALTER DATABASE [OPCRUX_AD] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [OPCRUX_AD] SET DB_CHAINING OFF 
GO
ALTER DATABASE [OPCRUX_AD] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [OPCRUX_AD] SET TARGET_RECOVERY_TIME = 60 SECONDS 
GO
ALTER DATABASE [OPCRUX_AD] SET DELAYED_DURABILITY = DISABLED 
GO
EXEC sys.sp_db_vardecimal_storage_format N'OPCRUX_AD', N'ON'
GO
ALTER DATABASE [OPCRUX_AD] SET QUERY_STORE = OFF
GO
USE [OPCRUX_AD]
GO
/****** Object:  User [dev_opcrux]    Script Date: 20.12.2019 16:13:56 ******/
CREATE USER [dev_opcrux] FOR LOGIN [dev_opcrux] WITH DEFAULT_SCHEMA=[dbo]
GO
ALTER ROLE [db_owner] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_datareader] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_datawriter] ADD MEMBER [dev_opcrux]
GO
/****** Object:  Table [dbo].[Accounts]    Script Date: 20.12.2019 16:13:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Accounts](
	[id] [int] IDENTITY(0,1) NOT NULL,
	[username] [varchar](33) NOT NULL,
	[password] [char](128) NOT NULL,
	[rank] [int] NULL
) ON [PRIMARY]
GO
USE [master]
GO
ALTER DATABASE [OPCRUX_AD] SET  READ_WRITE 
GO
