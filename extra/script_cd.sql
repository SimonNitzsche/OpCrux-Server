USE [master]
GO
/****** Object:  Database [OPCRUX_CD]    Script Date: 03.05.2020 18:34:59 ******/
CREATE DATABASE [OPCRUX_CD]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'OPCRUX_CD', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.SQLEXPRESS\MSSQL\DATA\OPCRUX_CD.mdf' , SIZE = 8192KB , MAXSIZE = UNLIMITED, FILEGROWTH = 65536KB )
 LOG ON 
( NAME = N'OPCRUX_CD_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.SQLEXPRESS\MSSQL\DATA\OPCRUX_CD_log.ldf' , SIZE = 8192KB , MAXSIZE = 2048GB , FILEGROWTH = 65536KB )
GO
ALTER DATABASE [OPCRUX_CD] SET COMPATIBILITY_LEVEL = 140
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [OPCRUX_CD].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [OPCRUX_CD] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET ARITHABORT OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [OPCRUX_CD] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [OPCRUX_CD] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET  DISABLE_BROKER 
GO
ALTER DATABASE [OPCRUX_CD] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [OPCRUX_CD] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET RECOVERY FULL 
GO
ALTER DATABASE [OPCRUX_CD] SET  MULTI_USER 
GO
ALTER DATABASE [OPCRUX_CD] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [OPCRUX_CD] SET DB_CHAINING OFF 
GO
ALTER DATABASE [OPCRUX_CD] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [OPCRUX_CD] SET TARGET_RECOVERY_TIME = 60 SECONDS 
GO
ALTER DATABASE [OPCRUX_CD] SET DELAYED_DURABILITY = DISABLED 
GO
ALTER DATABASE [OPCRUX_CD] SET QUERY_STORE = OFF
GO
USE [OPCRUX_CD]
GO
/****** Object:  User [dev_opcrux]    Script Date: 03.05.2020 18:34:59 ******/
CREATE USER [dev_opcrux] FOR LOGIN [dev_opcrux] WITH DEFAULT_SCHEMA=[dbo]
GO
ALTER ROLE [db_owner] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_accessadmin] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_securityadmin] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_ddladmin] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_backupoperator] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_datareader] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_datawriter] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_denydatareader] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_denydatawriter] ADD MEMBER [dev_opcrux]
GO
/****** Object:  Table [dbo].[IDCounter]    Script Date: 03.05.2020 18:34:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[IDCounter](
	[type] [varchar](50) NOT NULL,
	[counter] [bigint] NOT NULL
) ON [PRIMARY]
GO
USE [master]
GO
ALTER DATABASE [OPCRUX_CD] SET  READ_WRITE 
GO
