USE [master]
GO
/****** Object:  Database [OPCRUX_KD]    Script Date: 04.08.2019 16:03:11 ******/
CREATE DATABASE [OPCRUX_KD]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'OPCRUX_KD', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER\MSSQL\DATA\OPCRUX_KD.mdf' , SIZE = 8192KB , MAXSIZE = UNLIMITED, FILEGROWTH = 65536KB )
 LOG ON 
( NAME = N'OPCRUX_KD_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER\MSSQL\DATA\OPCRUX_KD_log.ldf' , SIZE = 8192KB , MAXSIZE = 2048GB , FILEGROWTH = 65536KB )
GO
ALTER DATABASE [OPCRUX_KD] SET COMPATIBILITY_LEVEL = 140
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [OPCRUX_KD].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [OPCRUX_KD] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET ARITHABORT OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [OPCRUX_KD] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [OPCRUX_KD] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET  DISABLE_BROKER 
GO
ALTER DATABASE [OPCRUX_KD] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [OPCRUX_KD] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET RECOVERY FULL 
GO
ALTER DATABASE [OPCRUX_KD] SET  MULTI_USER 
GO
ALTER DATABASE [OPCRUX_KD] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [OPCRUX_KD] SET DB_CHAINING OFF 
GO
ALTER DATABASE [OPCRUX_KD] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [OPCRUX_KD] SET TARGET_RECOVERY_TIME = 60 SECONDS 
GO
ALTER DATABASE [OPCRUX_KD] SET DELAYED_DURABILITY = DISABLED 
GO
EXEC sys.sp_db_vardecimal_storage_format N'OPCRUX_KD', N'ON'
GO
ALTER DATABASE [OPCRUX_KD] SET QUERY_STORE = OFF
GO
USE [OPCRUX_KD]
GO
/****** Object:  User [dev_opcrux]    Script Date: 04.08.2019 16:03:11 ******/
CREATE USER [dev_opcrux] FOR LOGIN [dev_opcrux] WITH DEFAULT_SCHEMA=[dbo]
GO
ALTER ROLE [db_accessadmin] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_datareader] ADD MEMBER [dev_opcrux]
GO
ALTER ROLE [db_datawriter] ADD MEMBER [dev_opcrux]
GO
USE [master]
GO
ALTER DATABASE [OPCRUX_KD] SET  READ_WRITE 
GO
