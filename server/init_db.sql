CREATE DATABASE IF NOT EXISTS `firefly_helloworld`;

USE `firefly_helloworld`;

CREATE TABLE IF NOT EXISTS `user` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL COMMENT 'user name',
  `password` varchar(255) NOT NULL COMMENT 'user password md5',
  `device_id` varchar(255) NOT NULL COMMENT 'device unique id',
  `blocked` int(1) NOT NULL DEFAULT 0 COMMENT '1 means user is blocked',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `message` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `user_id` int(10) NOT NULL COMMENT 'user id',
  `message` varchar(255) NOT NULL COMMENT 'user message sent to server',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;