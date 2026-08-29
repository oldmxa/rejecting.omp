SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

CREATE TABLE `accounts` (
  `id` int(11) NOT NULL,
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
  `hud_shade_one` int(11) NOT NULL DEFAULT -1,
  `hud_shade_two` int(11) NOT NULL DEFAULT -1,
  `chat_color` int(11) NOT NULL DEFAULT 0,
  `chat_id_color` int(11) NOT NULL DEFAULT 0,
  `chat_main_color` int(11) NOT NULL DEFAULT 0,
  `hud_type` int(11) NOT NULL DEFAULT 0,
  `life_type` int(11) NOT NULL DEFAULT 0,
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
  `show_damage_informer` tinyint(1) NOT NULL DEFAULT 1,
  `staticdamage` int(11) NOT NULL DEFAULT 0,
  `showplayerhud` int(11) NOT NULL DEFAULT 1,
  `show_clan_hud` tinyint(1) NOT NULL DEFAULT 1,
  `show_headshot` int(11) NOT NULL DEFAULT 0,
  `show_connect_msg` int(11) NOT NULL DEFAULT 1,
  `hide_global_msg` int(11) NOT NULL DEFAULT 0,
  `hide_welcome_msg` tinyint(1) NOT NULL DEFAULT 0,
  `show_3d_deathtag` tinyint(1) NOT NULL DEFAULT 0,
  `show_dmgfeed` tinyint(1) NOT NULL DEFAULT 1,
  `dmgfeed_style` tinyint(1) NOT NULL DEFAULT 1,
  `dmgfeed_color_left` int(11) NOT NULL DEFAULT -1,
  `dmgfeed_color_right` int(11) NOT NULL DEFAULT -1,
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
  `avatar_url` varchar(255) DEFAULT 'assets/images/default-avatar.jpg',
  `profile_banner_url` varchar(255) DEFAULT NULL,
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
  `forced_omp` int(11) NOT NULL DEFAULT 0,
  `hide_vip_tag` tinyint(1) NOT NULL DEFAULT 0,
  `hide_clan_tag` tinyint(1) NOT NULL DEFAULT 0,
  `hide_admin_tag` tinyint(1) NOT NULL DEFAULT 1,
  `hide_custom_tag` tinyint(1) NOT NULL DEFAULT 0,
  `custom_tag` varchar(32) NOT NULL DEFAULT '',
  `custom_tag_perm` tinyint(1) NOT NULL DEFAULT 0,
  `spawn_safehouse` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `ac_detections` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL DEFAULT 0,
  `username` varchar(24) NOT NULL DEFAULT '',
  `code` int(11) NOT NULL DEFAULT 0,
  `action` int(11) NOT NULL DEFAULT 0,
  `ip` varchar(46) NOT NULL DEFAULT '',
  `gpci` varchar(41) NOT NULL DEFAULT '',
  `created_at` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `ac_config` (
  `key` varchar(32) NOT NULL,
  `value` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `ac_detection_totals` (
  `user_id` int(11) NOT NULL DEFAULT 0,
  `code` int(11) NOT NULL DEFAULT 0,
  `hits` int(11) NOT NULL DEFAULT 0,
  `last_seen` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `admin_notes` (
  `id` int(11) NOT NULL,
  `target_id` int(11) NOT NULL,
  `admin_id` int(11) NOT NULL,
  `note` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `arena_logs` (
  `id` int(11) NOT NULL,
  `arena_id` int(11) NOT NULL,
  `arena_name` varchar(64) NOT NULL,
  `winner_name` varchar(24) DEFAULT NULL,
  `winner_clan` varchar(24) DEFAULT NULL,
  `total_kills` int(11) NOT NULL DEFAULT 0,
  `total_damage` int(11) NOT NULL DEFAULT 0,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `banimentos` (
  `id` int(11) NOT NULL,
  `username` varchar(25) NOT NULL,
  `ip` varchar(30) DEFAULT NULL,
  `admin` varchar(50) NOT NULL,
  `ban_timestamp` int(64) NOT NULL,
  `unban_timestamp` int(64) NOT NULL,
  `reason` varchar(64) NOT NULL,
  `country` varchar(50) DEFAULT NULL,
  `isp` varchar(100) DEFAULT NULL,
  `gpci` varchar(41) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `changelog` (
  `id` int(11) NOT NULL,
  `title` varchar(120) NOT NULL,
  `content` mediumtext NOT NULL,
  `author_id` int(11) NOT NULL,
  `author_name` varchar(100) NOT NULL,
  `is_pinned` tinyint(1) NOT NULL DEFAULT 0,
  `is_hidden` tinyint(1) NOT NULL DEFAULT 0,
  `is_priority` tinyint(1) NOT NULL DEFAULT 0,
  `views` int(11) NOT NULL DEFAULT 0,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `updated_at` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `chat_logs` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `username` varchar(24) NOT NULL,
  `channel` varchar(20) NOT NULL,
  `message` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `coins_logs` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `username` varchar(100) DEFAULT NULL,
  `reason` varchar(64) DEFAULT NULL,
  `amount` int(11) NOT NULL,
  `balance` int(11) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

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
  `clan_avatar` varchar(255) DEFAULT 'assets/images/default-avatar.jpg'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci ROW_FORMAT=COMPACT;

CREATE TABLE `info_posts` (
  `id` int(11) NOT NULL,
  `section` varchar(40) NOT NULL,
  `title` varchar(120) NOT NULL,
  `content` mediumtext NOT NULL,
  `link_url` varchar(500) DEFAULT NULL,
  `link_label` varchar(60) DEFAULT NULL,
  `author_id` int(11) NOT NULL,
  `author_name` varchar(100) NOT NULL,
  `is_pinned` tinyint(1) NOT NULL DEFAULT 0,
  `is_locked` tinyint(1) NOT NULL DEFAULT 0,
  `is_hidden` tinyint(1) NOT NULL DEFAULT 0,
  `is_priority` tinyint(1) NOT NULL DEFAULT 0,
  `views` int(11) NOT NULL DEFAULT 0,
  `replies` int(11) NOT NULL DEFAULT 0,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `updated_at` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp(),
  `deleted_at` timestamp NULL DEFAULT NULL,
  `deleted_by_id` int(11) DEFAULT NULL,
  `deleted_by_name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `info_replies` (
  `id` int(11) NOT NULL,
  `post_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `username` varchar(100) NOT NULL,
  `content` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `deleted_at` timestamp NULL DEFAULT NULL,
  `deleted_by_id` int(11) DEFAULT NULL,
  `deleted_by_name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `ip_whitelist` (
  `id` int(11) NOT NULL,
  `ip` varchar(18) NOT NULL,
  `username` varchar(24) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `ispsbanidas` (
  `id` int(11) NOT NULL,
  `isp` varchar(150) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `name_changes` (
  `id` int(11) NOT NULL,
  `uid` int(10) UNSIGNED NOT NULL,
  `old_name` varchar(20) NOT NULL,
  `new_name` varchar(20) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `outposts` (
  `zoneid` int(11) NOT NULL,
  `outpostid` int(11) NOT NULL,
  `clanid` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `paisesbanidos` (
  `country_name` varchar(35) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `player_daily_stats` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `kills` int(11) NOT NULL DEFAULT 0,
  `deaths` int(11) NOT NULL DEFAULT 0,
  `arenas_won` int(11) NOT NULL DEFAULT 0,
  `zone_points` int(11) NOT NULL DEFAULT 0,
  `date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `player_match_history` (
  `id` int(11) NOT NULL,
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
  `played_at` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `safehouses` (
  `id` int(11) NOT NULL,
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
  `status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `safehouse_config` (
  `key` varchar(32) NOT NULL,
  `value` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `safehouse_logs` (
  `id` int(11) NOT NULL,
  `safehouse_id` int(11) NOT NULL DEFAULT 0,
  `zone_id` int(11) NOT NULL DEFAULT -1,
  `clan_id` int(11) NOT NULL DEFAULT -1,
  `user_id` int(11) NOT NULL DEFAULT 0,
  `player_name` varchar(24) NOT NULL DEFAULT '',
  `action` varchar(32) NOT NULL DEFAULT '',
  `detail` varchar(160) NOT NULL DEFAULT '',
  `ip` varchar(46) NOT NULL DEFAULT '',
  `created_at` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `server_logs` (
  `id` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `username` varchar(100) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `action` varchar(255) DEFAULT NULL,
  `ticket_id` int(11) DEFAULT NULL,
  `panel` varchar(100) DEFAULT NULL,
  `details` text DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `site_config` (
  `key` varchar(32) NOT NULL,
  `value` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

INSERT INTO `site_config` (`key`, `value`) VALUES
('applications_open', 0),
('ideas_open', 0);

CREATE TABLE `server_stats` (
  `id` int(11) NOT NULL,
  `online_count` int(11) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

INSERT INTO `server_stats` (`id`, `online_count`, `created_at`) VALUES
(1, 0, '2026-08-09 07:01:40');

CREATE TABLE `server_status` (
  `id` int(11) NOT NULL,
  `current_arena_id` int(11) DEFAULT -1,
  `current_arena_name` varchar(64) DEFAULT 'Lounge',
  `players_online` int(11) DEFAULT 0,
  `last_updated` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

INSERT INTO `server_status` (`id`, `current_arena_id`, `current_arena_name`, `players_online`) VALUES
(1, -1, 'Lounge', 0);

CREATE TABLE `staff_applications` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `username` varchar(100) NOT NULL,
  `real_name` varchar(80) NOT NULL DEFAULT '',
  `age` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `country` varchar(60) NOT NULL DEFAULT '',
  `study_work` varchar(255) NOT NULL DEFAULT '',
  `play_period` varchar(40) NOT NULL DEFAULT '',
  `hours_per_day` varchar(40) NOT NULL DEFAULT '',
  `past_admin` text DEFAULT NULL,
  `level_at_apply` int(11) NOT NULL DEFAULT 0,
  `played_time_at_apply` int(11) NOT NULL DEFAULT 0,
  `time_playing` varchar(255) NOT NULL,
  `motivation` text NOT NULL,
  `status` enum('pending','approved','rejected') NOT NULL DEFAULT 'pending',
  `reviewed_by_id` int(11) DEFAULT NULL,
  `reviewed_by_name` varchar(100) DEFAULT NULL,
  `review_note` varchar(500) DEFAULT NULL,
  `reviewed_at` timestamp NULL DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `tickets` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `subject` varchar(255) NOT NULL,
  `description` text NOT NULL,
  `priority` enum('low','medium','high') DEFAULT 'medium',
  `status` enum('open','closed','pending') DEFAULT 'open',
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `accused_name` varchar(100) DEFAULT NULL,
  `ticket_type` varchar(50) DEFAULT 'support'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `ticket_messages` (
  `id` int(11) NOT NULL,
  `ticket_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `message` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `training_records` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `arena_id` int(11) NOT NULL DEFAULT 0,
  `arena_name` varchar(32) NOT NULL DEFAULT '',
  `best_score` int(11) NOT NULL DEFAULT 0,
  `best_hits` int(11) NOT NULL DEFAULT 0,
  `best_shots` int(11) NOT NULL DEFAULT 0,
  `best_damage` int(11) NOT NULL DEFAULT 0,
  `set_at` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `videos` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `title` varchar(255) NOT NULL,
  `video_id` varchar(50) NOT NULL,
  `participants` varchar(255) DEFAULT NULL,
  `description` text DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `web_commands` (
  `id` int(11) NOT NULL,
  `admin_id` int(11) NOT NULL,
  `admin_name` varchar(100) NOT NULL,
  `admin_level` int(11) NOT NULL DEFAULT 0,
  `command` varchar(160) NOT NULL,
  `status` enum('pending','running','done','failed') NOT NULL DEFAULT 'pending',
  `response` varchar(500) DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `executed_at` timestamp NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `zones` (
  `id` int(11) NOT NULL,
  `zoneid` int(11) NOT NULL,
  `clanid` int(11) NOT NULL,
  `dmg` int(11) NOT NULL,
  `clancolor` varchar(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `tournaments` (
  `id` int(11) NOT NULL,
  `tournament_name` varchar(64) NOT NULL DEFAULT 'Rejecting Championship',
  `achievement_tag` varchar(32) NOT NULL DEFAULT 'CHAMPION',
  `mode` tinyint(4) NOT NULL DEFAULT 1,
  `max_slots` int(11) NOT NULL DEFAULT 8,
  `prize_coins` int(11) NOT NULL DEFAULT 0,
  `prize_money` int(11) NOT NULL DEFAULT 0,
  `entry_fee` int(11) NOT NULL DEFAULT 0,
  `arena_id` int(11) NOT NULL DEFAULT -1,
  `winner_name` varchar(64) DEFAULT NULL,
  `started_by` varchar(32) NOT NULL DEFAULT 'System',
  `scheduled_time` varchar(64) NOT NULL DEFAULT 'Now',
  `created_at` int(11) NOT NULL DEFAULT 0,
  `finished_at` int(11) NOT NULL DEFAULT 0,
  `status` varchar(20) NOT NULL DEFAULT 'finished'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

CREATE TABLE `player_tournament_tags` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `tag_name` varchar(32) NOT NULL,
  `tournament_name` varchar(64) NOT NULL,
  `won_at` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

ALTER TABLE `accounts`
  ADD PRIMARY KEY (`id`),
  ADD KEY `username` (`username`),
  ADD KEY `kills` (`kills`),
  ADD KEY `kills_semanais` (`kills_semanais`),
  ADD KEY `damage_semanal` (`damage_semanal`),
  ADD KEY `clanid` (`clanid`),
  ADD KEY `regip` (`regip`),
  ADD KEY `dc_username` (`dc_username`),
  ADD KEY `dc_uid` (`dc_uid`),
  ADD KEY `idx_accounts_unmute` (`unmute_timestamp`),
  ADD KEY `idx_accounts_warnings` (`warnings`);

ALTER TABLE `ac_detections`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user` (`user_id`),
  ADD KEY `code` (`code`);

ALTER TABLE `ac_config`
  ADD PRIMARY KEY (`key`);

ALTER TABLE `ac_detection_totals`
  ADD PRIMARY KEY (`user_id`,`code`),
  ADD KEY `code` (`code`);

ALTER TABLE `admin_notes`
  ADD PRIMARY KEY (`id`),
  ADD KEY `target_id` (`target_id`),
  ADD KEY `admin_id` (`admin_id`);

ALTER TABLE `arena_logs`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `banimentos`
  ADD PRIMARY KEY (`id`),
  ADD KEY `username` (`username`),
  ADD KEY `ip` (`ip`),
  ADD KEY `gpci` (`gpci`),
  ADD KEY `idx_ban_unban` (`unban_timestamp`);

ALTER TABLE `changelog`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_listagem` (`is_pinned`,`created_at`);

ALTER TABLE `chat_logs`
  ADD PRIMARY KEY (`id`),
  ADD KEY `channel_idx` (`channel`),
  ADD KEY `created_at_idx` (`created_at`);

ALTER TABLE `coins_logs`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id_idx` (`user_id`),
  ADD KEY `created_at_idx` (`created_at`);

ALTER TABLE `clans`
  ADD PRIMARY KEY (`clan_id`),
  ADD KEY `clan_tag` (`clan_tag`),
  ADD KEY `clan_name` (`clan_name`);

ALTER TABLE `info_posts`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_secao` (`section`,`is_pinned`,`created_at`),
  ADD KEY `idx_vivos` (`section`,`deleted_at`,`is_pinned`,`created_at`);

ALTER TABLE `info_replies`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_post` (`post_id`,`deleted_at`,`created_at`);

ALTER TABLE `ip_whitelist`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `ispsbanidas`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `isp` (`isp`);

ALTER TABLE `name_changes`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `outposts`
  ADD PRIMARY KEY (`zoneid`,`outpostid`);

ALTER TABLE `paisesbanidos`
  ADD UNIQUE KEY `country_name` (`country_name`) USING BTREE;

ALTER TABLE `player_daily_stats`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `uniq_user_date` (`user_id`,`date`),
  ADD KEY `idx_user_date` (`user_id`,`date`);

ALTER TABLE `player_match_history`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `uniq_user_arena_round` (`user_id`,`arena_id`,`played_at`),
  ADD KEY `idx_user_played` (`user_id`,`played_at`);

ALTER TABLE `safehouses`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `zone` (`zone_id`);

ALTER TABLE `safehouse_config`
  ADD PRIMARY KEY (`key`);

ALTER TABLE `safehouse_logs`
  ADD PRIMARY KEY (`id`),
  ADD KEY `safehouse` (`safehouse_id`),
  ADD KEY `clan` (`clan_id`);

ALTER TABLE `server_logs`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_server_logs_category` (`category`),
  ADD KEY `idx_server_logs_user` (`user_id`),
  ADD KEY `idx_server_logs_created` (`created_at`);

ALTER TABLE `site_config`
  ADD PRIMARY KEY (`key`);

ALTER TABLE `server_stats`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `server_status`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `staff_applications`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_status` (`status`,`created_at`),
  ADD KEY `idx_autor` (`user_id`,`status`);

ALTER TABLE `tickets`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_tickets_type_status` (`ticket_type`, `status`),
  ADD KEY `idx_tickets_user` (`user_id`),
  ADD KEY `idx_tickets_created` (`created_at`);

ALTER TABLE `ticket_messages`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_ticket_msg_ticket` (`ticket_id`);

ALTER TABLE `training_records`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `user_arena` (`user_id`,`arena_name`);

ALTER TABLE `videos`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `web_commands`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_fila` (`status`,`id`),
  ADD KEY `idx_recentes` (`created_at`);

ALTER TABLE `zones`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `tournaments`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_tourney_created` (`created_at`);

ALTER TABLE `player_tournament_tags`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `idx_user_tag` (`user_id`,`tag_name`),
  ADD KEY `idx_tag_won_at` (`won_at`);

ALTER TABLE `accounts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `ac_detections`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `admin_notes`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `arena_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `banimentos`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `changelog`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `chat_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `coins_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `ip_whitelist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `info_posts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `info_replies`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `ispsbanidas`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `name_changes`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `player_daily_stats`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `player_match_history`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `safehouses`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `safehouse_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `server_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `server_stats`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

ALTER TABLE `server_status`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `staff_applications`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `tickets`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `ticket_messages`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `training_records`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `videos`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `web_commands`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `zones`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `tournaments`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `player_tournament_tags`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
