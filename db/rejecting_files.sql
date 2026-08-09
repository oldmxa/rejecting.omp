-- MariaDB dump 10.19  Distrib 10.4.32-MariaDB, for Win64 (AMD64)
--
-- Host: localhost    Database: rejecting.files
-- ------------------------------------------------------
-- Server version	10.4.32-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `accounts`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `accounts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(24) NOT NULL,
  `password` varchar(61) NOT NULL,
  `regip` varchar(17) DEFAULT NULL,
  `lastip` varchar(17) DEFAULT NULL,
  `level` int(11) NOT NULL DEFAULT 1,
  `kills` int(11) NOT NULL DEFAULT 0,
  `kills_semanais` int(11) NOT NULL DEFAULT 0,
  `deaths` int(11) NOT NULL DEFAULT 0,
  `max_damage` int(11) NOT NULL DEFAULT 0,
  `damage_semanal` float NOT NULL DEFAULT 0,
  `max_streak` int(11) NOT NULL DEFAULT 0,
  `weather` int(11) NOT NULL DEFAULT 10,
  `time` int(11) NOT NULL DEFAULT 10,
  `hitmarkcolor` int(10) NOT NULL DEFAULT -52,
  `hudcolor` int(11) NOT NULL DEFAULT 0,
  `chat_color` int(11) NOT NULL DEFAULT 0,
  `chat_id_color` int(11) NOT NULL DEFAULT 0,
  `hud_type` int(11) NOT NULL DEFAULT 0,
  `adminlevel` int(11) NOT NULL DEFAULT 0,
  `skin_id` int(11) NOT NULL DEFAULT 12,
  `fighting_style` int(11) NOT NULL DEFAULT 4,
  `unmute_timestamp` int(64) NOT NULL DEFAULT 0,
  `last_login` int(64) NOT NULL DEFAULT 0,
  `block_pm` int(11) NOT NULL DEFAULT 0,
  `player_weapons` varchar(129) DEFAULT '1,2,0,0,',
  `player_money` int(11) NOT NULL DEFAULT 0,
  `coins` int(11) NOT NULL DEFAULT 0,
  `warnings` int(11) NOT NULL DEFAULT 0,
  `played_time` int(11) NOT NULL DEFAULT 0,
  `won_arenas` int(11) NOT NULL DEFAULT 0,
  `duel_wins` int(11) NOT NULL DEFAULT 0,
  `duel_losses` int(11) NOT NULL DEFAULT 0,
  `duel2v2_wins` int(11) NOT NULL DEFAULT 0,
  `duel2v2_losses` int(11) NOT NULL DEFAULT 0,
  `gpci` varchar(42) NOT NULL DEFAULT '',
  `clanid` int(11) NOT NULL DEFAULT -1,
  `clanrole` int(11) NOT NULL DEFAULT -1,
  `country` varchar(50) DEFAULT NULL,
  `isp` varchar(100) DEFAULT NULL,
  `city` varchar(64) DEFAULT NULL,
  `region` varchar(64) DEFAULT NULL,
  `regioncode` varchar(64) DEFAULT NULL,
  `countrycode` varchar(64) DEFAULT NULL,
  `latitude` varchar(64) DEFAULT NULL,
  `longitude` varchar(64) DEFAULT NULL,
  `timezone` varchar(64) DEFAULT NULL,
  `network` varchar(64) DEFAULT NULL,
  `ispnumber` varchar(64) DEFAULT NULL,
  `vpn` int(11) NOT NULL DEFAULT 0,
  `proxy` int(11) NOT NULL DEFAULT 0,
  `staticdamage` int(11) NOT NULL DEFAULT 0,
  `showplayerhud` int(11) NOT NULL DEFAULT 1,
  `show_clan_hud` tinyint(1) NOT NULL DEFAULT 1,
  `show_headshot` int(11) NOT NULL DEFAULT 0,
  `show_connect_msg` int(11) NOT NULL DEFAULT 1,
  `hide_global_msg` int(11) NOT NULL DEFAULT 0,
  `hide_welcome_msg` tinyint(1) NOT NULL DEFAULT 0,
  `show_3d_deathtag` tinyint(1) NOT NULL DEFAULT 1,
  `show_random_deathtag` tinyint(1) NOT NULL DEFAULT 1,
  `hit_sound_id` int(11) NOT NULL DEFAULT 17802,
  `kill_sound_id` int(11) NOT NULL DEFAULT 1057,
  `acfg_pm` int(2) NOT NULL DEFAULT 0,
  `acfg_clanpm` int(2) NOT NULL DEFAULT 0,
  `acfg_cmds` int(2) NOT NULL DEFAULT 0,
  `acfg_local` int(2) NOT NULL DEFAULT 0,
  `admin_auth_type` int(11) NOT NULL DEFAULT 0,
  `admin_password` varchar(65) NOT NULL DEFAULT '',
  `vip_level` int(11) NOT NULL DEFAULT 0,
  `vip_expire` int(64) NOT NULL DEFAULT 0,
  `vip_head_tag` int(11) NOT NULL DEFAULT 1,
  `lounge_x` float NOT NULL DEFAULT 1208.08,
  `lounge_y` float NOT NULL DEFAULT -2036.75,
  `lounge_z` float NOT NULL DEFAULT 69.0078,
  `lounge_a` float NOT NULL DEFAULT 270,
  `lounge_vw` int(11) NOT NULL DEFAULT 909,
  `lounge_int` int(11) NOT NULL DEFAULT 1,
  `dc_username` varchar(100) DEFAULT '-1',
  `dc_discriminator` varchar(100) DEFAULT '-1',
  `dc_uid` varchar(100) DEFAULT '-1',
  `avatar_url` varchar(255) DEFAULT NULL,
  `is_online` tinyint(1) NOT NULL DEFAULT 0,
  `last_activity` datetime DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `role` varchar(20) DEFAULT 'user',
  `auto_login` tinyint(1) NOT NULL DEFAULT 0,
  `profile_status` varchar(100) DEFAULT NULL,
  `profile_bio` text DEFAULT NULL,
  `social_youtube` varchar(100) DEFAULT NULL,
  `social_steam` varchar(100) DEFAULT NULL,
  `achievements_data` mediumtext NOT NULL DEFAULT '',
  `videos` int(11) NOT NULL DEFAULT 0,
  `forcedr2` int(11) NOT NULL DEFAULT 0,
  `showhud` int(11) NOT NULL DEFAULT 1,
  `hide_vip_tag` tinyint(1) NOT NULL DEFAULT 0,
  `hide_clan_tag` tinyint(1) NOT NULL DEFAULT 0,
  `hide_admin_tag` tinyint(1) NOT NULL DEFAULT 1,
  `hide_custom_tag` tinyint(1) NOT NULL DEFAULT 0,
  `custom_tag` varchar(32) NOT NULL DEFAULT '',
  `custom_tag_perm` tinyint(1) NOT NULL DEFAULT 0,
  `spawn_safehouse` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `username` (`username`),
  KEY `kills` (`kills`),
  KEY `kills_semanais` (`kills_semanais`),
  KEY `damage_semanal` (`damage_semanal`),
  KEY `clanid` (`clanid`),
  KEY `regip` (`regip`),
  KEY `dc_username` (`dc_username`),
  KEY `dc_uid` (`dc_uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `admin_logs`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `admin_logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `admin_name` varchar(24) NOT NULL,
  `target_name` varchar(24) DEFAULT NULL,
  `action` varchar(255) NOT NULL,
  `details` text DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `admin_notes`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `admin_notes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `target_id` int(11) NOT NULL,
  `admin_id` int(11) NOT NULL,
  `note` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `target_id` (`target_id`),
  KEY `admin_id` (`admin_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `arena_logs`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `arena_logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `arena_id` int(11) NOT NULL,
  `arena_name` varchar(64) NOT NULL,
  `winner_name` varchar(24) DEFAULT NULL,
  `winner_clan` varchar(24) DEFAULT NULL,
  `total_kills` int(11) NOT NULL DEFAULT 0,
  `total_damage` int(11) NOT NULL DEFAULT 0,
  `timestamp` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `banimentos`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `banimentos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(25) NOT NULL,
  `ip` varchar(30) DEFAULT NULL,
  `admin` varchar(50) NOT NULL,
  `ban_timestamp` int(64) NOT NULL,
  `unban_timestamp` int(64) NOT NULL,
  `reason` varchar(64) NOT NULL,
  `country` varchar(50) DEFAULT NULL,
  `isp` varchar(100) DEFAULT NULL,
  `gpci` varchar(41) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `username` (`username`),
  KEY `ip` (`ip`),
  KEY `gpci` (`gpci`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `chat_logs`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chat_logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `username` varchar(24) NOT NULL,
  `channel` varchar(20) NOT NULL,
  `message` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `channel_idx` (`channel`),
  KEY `created_at_idx` (`created_at`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clans`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clans` (
  `clan_id` int(11) NOT NULL,
  `clan_leader` int(11) NOT NULL,
  `clan_name` varchar(20) NOT NULL,
  `clan_tag` varchar(5) NOT NULL,
  `clan_roles` varchar(500) NOT NULL DEFAULT 'Trial,Member,Recruter,Co Founder,Founder',
  `clan_zone_points` int(11) NOT NULL DEFAULT 0,
  `clan_kills` int(11) NOT NULL DEFAULT 0,
  `clan_color` int(11) NOT NULL DEFAULT -1,
  `clan_created_at` int(11) NOT NULL DEFAULT 0,
  `clan_avatar` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`clan_id`),
  KEY `clan_tag` (`clan_tag`),
  KEY `clan_name` (`clan_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ip_whitelist`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ip_whitelist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(18) NOT NULL,
  `username` varchar(24) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ispsbanidas`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ispsbanidas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `isp` varchar(150) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `isp` (`isp`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `name_changes`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `name_changes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `uid` int(10) unsigned NOT NULL,
  `old_name` varchar(20) NOT NULL,
  `new_name` varchar(20) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `new_arena`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_arena` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `authorid` int(10) unsigned NOT NULL,
  `name` varchar(30) NOT NULL,
  `minx` float NOT NULL,
  `miny` float NOT NULL,
  `maxx` float NOT NULL,
  `maxy` float NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `outposts`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `outposts` (
  `zoneid` int(11) NOT NULL,
  `outpostid` int(11) NOT NULL,
  `clanid` int(11) NOT NULL,
  PRIMARY KEY (`zoneid`,`outpostid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `paisesbanidos`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `paisesbanidos` (
  `country_name` varchar(35) NOT NULL,
  UNIQUE KEY `country_name` (`country_name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_daily_stats`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_daily_stats` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `kills` int(11) NOT NULL DEFAULT 0,
  `deaths` int(11) NOT NULL DEFAULT 0,
  `arenas_won` int(11) NOT NULL DEFAULT 0,
  `zone_points` int(11) NOT NULL DEFAULT 0,
  `date` date NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniq_user_date` (`user_id`,`date`),
  KEY `idx_user_date` (`user_id`,`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_match_history`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_match_history` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `arena_id` int(11) NOT NULL,
  `arena_name` varchar(64) NOT NULL DEFAULT '',
  `kills` int(11) NOT NULL DEFAULT 0,
  `deaths` int(11) NOT NULL DEFAULT 0,
  `headshots` int(11) NOT NULL DEFAULT 0,
  `damage` int(11) NOT NULL DEFAULT 0,
  `shots_fired` int(11) NOT NULL DEFAULT 0,
  `shots_hit` int(11) NOT NULL DEFAULT 0,
  `exp_gained` int(11) NOT NULL DEFAULT 0,
  `placement` int(11) NOT NULL DEFAULT 0,
  `participants` int(11) NOT NULL DEFAULT 0,
  `player_level` int(11) NOT NULL DEFAULT 0,
  `played_at` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniq_user_arena_round` (`user_id`,`arena_id`,`played_at`),
  KEY `idx_user_played` (`user_id`,`played_at`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `safehouse_config`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `safehouse_config` (
  `key` varchar(32) NOT NULL,
  `value` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `safehouse_logs`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `safehouse_logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `safehouse_id` int(11) NOT NULL DEFAULT 0,
  `zone_id` int(11) NOT NULL DEFAULT -1,
  `clan_id` int(11) NOT NULL DEFAULT -1,
  `user_id` int(11) NOT NULL DEFAULT 0,
  `player_name` varchar(24) NOT NULL DEFAULT '',
  `action` varchar(32) NOT NULL DEFAULT '',
  `detail` varchar(160) NOT NULL DEFAULT '',
  `ip` varchar(46) NOT NULL DEFAULT '',
  `created_at` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `safehouse` (`safehouse_id`),
  KEY `clan` (`clan_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `safehouses`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `safehouses` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `zone_id` int(11) NOT NULL,
  `clan_id` int(11) NOT NULL DEFAULT -1,
  `name` varchar(32) NOT NULL DEFAULT '',
  `password` varchar(32) NOT NULL DEFAULT '',
  `level` int(11) NOT NULL DEFAULT 1,
  `interior` int(11) NOT NULL DEFAULT 0,
  `virtual_world` int(11) NOT NULL DEFAULT 0,
  `ext_x` float NOT NULL DEFAULT 0,
  `ext_y` float NOT NULL DEFAULT 0,
  `ext_z` float NOT NULL DEFAULT 0,
  `int_x` float NOT NULL DEFAULT 0,
  `int_y` float NOT NULL DEFAULT 0,
  `int_z` float NOT NULL DEFAULT 0,
  `int_a` float NOT NULL DEFAULT 0,
  `exit_x` float NOT NULL DEFAULT 0,
  `exit_y` float NOT NULL DEFAULT 0,
  `exit_z` float NOT NULL DEFAULT 0,
  `vault` int(11) NOT NULL DEFAULT 0,
  `bought_at` int(11) NOT NULL DEFAULT 0,
  `expires_at` int(11) NOT NULL DEFAULT 0,
  `next_charge` int(11) NOT NULL DEFAULT 0,
  `last_charge` int(11) NOT NULL DEFAULT 0,
  `status` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `zone` (`zone_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `server_logs`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) DEFAULT NULL,
  `username` varchar(100) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `action` varchar(255) DEFAULT NULL,
  `ticket_id` int(11) DEFAULT NULL,
  `panel` varchar(100) DEFAULT NULL,
  `details` text DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `server_stats`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_stats` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `online_count` int(11) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `server_status`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_status` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `current_arena_id` int(11) DEFAULT -1,
  `current_arena_name` varchar(64) DEFAULT 'Lounge',
  `players_online` int(11) DEFAULT 0,
  `last_updated` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ticket_messages`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ticket_messages` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ticket_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `message` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tickets`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tickets` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `subject` varchar(255) NOT NULL,
  `description` text NOT NULL,
  `priority` enum('low','medium','high') DEFAULT 'medium',
  `status` enum('open','closed','pending') DEFAULT 'open',
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `accused_name` varchar(100) DEFAULT NULL,
  `ticket_type` varchar(50) DEFAULT 'support',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `training_records`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `training_records` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `arena_id` int(11) NOT NULL DEFAULT 0,
  `arena_name` varchar(32) NOT NULL DEFAULT '',
  `best_score` int(11) NOT NULL DEFAULT 0,
  `best_hits` int(11) NOT NULL DEFAULT 0,
  `best_shots` int(11) NOT NULL DEFAULT 0,
  `best_damage` int(11) NOT NULL DEFAULT 0,
  `set_at` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_arena` (`user_id`,`arena_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `videos`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `videos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `title` varchar(255) NOT NULL,
  `video_id` varchar(50) NOT NULL,
  `participants` varchar(255) DEFAULT NULL,
  `description` text DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zones`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zones` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `zoneid` int(11) NOT NULL,
  `clanid` int(11) NOT NULL,
  `dmg` int(11) NOT NULL,
  `clancolor` varchar(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-08-07 22:57:21
