/*
Navicat MySQL Data Transfer

Source Server         : 42.62.125.190
Source Server Version : 50550
Source Host           : 42.62.125.190:13579
Source Database       : mario

Target Server Type    : MYSQL
Target Server Version : 50550
File Encoding         : 65001

Date: 2016-11-20 08:38:43
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `mr_host`
-- ----------------------------
DROP TABLE IF EXISTS `mr_host`;
CREATE TABLE `mr_host` (
  `host_id` bigint(20) NOT NULL COMMENT '主机ID',
  `host` varchar(128) NOT NULL COMMENT '主机',
  `name` varchar(128) NOT NULL COMMENT '名称',
  `ip` varchar(15) NOT NULL COMMENT 'IP地址',
  `desc` varchar(255) DEFAULT 'NULL' COMMENT '描述',
  PRIMARY KEY (`host_id`),
  KEY `ip` (`ip`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mr_host
-- ----------------------------

-- ----------------------------
-- Table structure for `mr_pipeline`
-- ----------------------------
DROP TABLE IF EXISTS `mr_pipeline`;
CREATE TABLE `mr_pipeline` (
  `pl_id` bigint(20) NOT NULL COMMENT '流程ID',
  `pl_oldid` varchar(255) DEFAULT NULL COMMENT '原系统ID',
  `pl_name` varchar(50) NOT NULL COMMENT '名称',
  `pl_desc` varchar(255) NOT NULL COMMENT '说明',
  `pl_create_tm` datetime NOT NULL COMMENT '创建时间',
  `pl_modify_tm` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  PRIMARY KEY (`pl_id`),
  UNIQUE KEY `pl_name` (`pl_name`),
  UNIQUE KEY `pl_oldid` (`pl_oldid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mr_pipeline
-- ----------------------------

-- ----------------------------
-- Table structure for `mr_pl_exec`
-- ----------------------------
DROP TABLE IF EXISTS `mr_pl_exec`;
CREATE TABLE `mr_pl_exec` (
  `pe_id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '流程执行结果ID',
  `pl_id` bigint(20) NOT NULL COMMENT '流程ID',
  `pe_stm` datetime NOT NULL COMMENT '开始时间',
  `pe_etm` datetime DEFAULT NULL COMMENT '结束时间',
  `pe_desc` varchar(255) DEFAULT NULL COMMENT '说明',
  `pe_ret_code` int(11) NOT NULL COMMENT '返回值',
  `pe_ret_state` varchar(255) DEFAULT NULL COMMENT '执行结果或状态',
  `pe_type` enum('1','0') DEFAULT '0' COMMENT '执行类型：0,自动执行;1,手动执行',
  PRIMARY KEY (`pe_id`),
  KEY `pl_id` (`pl_id`),
  CONSTRAINT `mr_pl_exec_ibfk_1` FOREIGN KEY (`pl_id`) REFERENCES `mr_pipeline` (`pl_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mr_pl_exec
-- ----------------------------

-- ----------------------------
-- Table structure for `mr_pl_node`
-- ----------------------------
DROP TABLE IF EXISTS `mr_pl_node`;
CREATE TABLE `mr_pl_node` (
  `node_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `node_type` enum('1','0') NOT NULL DEFAULT '0' COMMENT '类型：0, 脚本; 1，流程',
  `pl_id` bigint(20) NOT NULL COMMENT '所属流程ID',
  `ref_id` bigint(20) NOT NULL COMMENT '引用ID',
  `src_id` bigint(20) NOT NULL COMMENT '引用源ID',
  `trg_id` bigint(20) NOT NULL COMMENT '引用目标ID',
  `src_type` enum('1','0') NOT NULL DEFAULT '0' COMMENT '类型：0, 脚本; 1，流程',
  `trg_type` enum('1','0') NOT NULL DEFAULT '0' COMMENT '类型：0, 脚本; 1，流程',
  `node_desc` varchar(255) NOT NULL COMMENT '说明',
  `create_tm` datetime NOT NULL COMMENT '创建时间',
  `modify_tm` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP COMMENT '修改时间',
  PRIMARY KEY (`node_id`),
  KEY `pl_id` (`pl_id`),
  CONSTRAINT `mr_pl_node_ibfk_1` FOREIGN KEY (`pl_id`) REFERENCES `mr_pipeline` (`pl_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mr_pl_node
-- ----------------------------

-- ----------------------------
-- Table structure for `mr_pln_exec`
-- ----------------------------
DROP TABLE IF EXISTS `mr_pln_exec`;
CREATE TABLE `mr_pln_exec` (
  `pln_id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '节点执行结果ID',
  `pe_id` bigint(20) NOT NULL COMMENT '流程执行ID',
  `pl_node` bigint(20) NOT NULL COMMENT '节点ID',
  `pln_stm` datetime NOT NULL COMMENT '开始时间',
  `pln_etm` datetime DEFAULT NULL COMMENT '结束时间',
  `pln_desc` varchar(255) DEFAULT NULL COMMENT '说明',
  `pln_ret_code` int(11) NOT NULL COMMENT '执行结果代码',
  `pln_ret_state` varchar(255) DEFAULT NULL COMMENT '执行结果或状态',
  PRIMARY KEY (`pln_id`),
  KEY `pl_node` (`pl_node`),
  KEY `pe_id` (`pe_id`),
  CONSTRAINT `mr_pln_exec_ibfk_2` FOREIGN KEY (`pe_id`) REFERENCES `mr_pl_exec` (`pe_id`),
  CONSTRAINT `mr_pln_exec_ibfk_1` FOREIGN KEY (`pl_node`) REFERENCES `mr_pl_node` (`node_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of mr_pln_exec
-- ----------------------------

-- ----------------------------
-- Table structure for `mr_scpt_host`
-- ----------------------------
DROP TABLE IF EXISTS `mr_scpt_host`;
CREATE TABLE `mr_scpt_host` (
  `scpt_host_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `host_id` bigint(20) NOT NULL,
  `scpt_id` bigint(20) NOT NULL,
  PRIMARY KEY (`scpt_host_id`),
  KEY `host_id` (`host_id`),
  CONSTRAINT `mr_scpt_host_ibfk_1` FOREIGN KEY (`host_id`) REFERENCES `mr_host` (`host_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mr_scpt_host
-- ----------------------------

-- ----------------------------
-- Table structure for `mr_script`
-- ----------------------------
DROP TABLE IF EXISTS `mr_script`;
CREATE TABLE `mr_script` (
  `scpt_id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '脚本ID',
  `scpt_name` varchar(50) NOT NULL COMMENT '脚本名称',
  `scpt_script` text NOT NULL COMMENT '脚本',
  `scpt_desc` varchar(255) NOT NULL COMMENT '说明',
  `scpt_create_tm` datetime NOT NULL COMMENT '创建时间',
  `scpt_modify_tm` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP COMMENT '修改时间',
  PRIMARY KEY (`scpt_id`),
  FULLTEXT KEY `scpt_name` (`scpt_name`),
  FULLTEXT KEY `scpt_script` (`scpt_script`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mr_script
-- ----------------------------

-- ----------------------------
-- View structure for `v_pl_exec`
-- ----------------------------
DROP VIEW IF EXISTS `v_pl_exec`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW `v_pl_exec` AS select `mr_pl_exec`.`pe_id` AS `pe_id`,`mr_pl_exec`.`pl_id` AS `pl_id`,`mr_pl_exec`.`pe_stm` AS `pe_stm`,`mr_pl_exec`.`pe_etm` AS `pe_etm`,`mr_pl_exec`.`pe_desc` AS `pe_desc`,`mr_pl_exec`.`pe_ret_code` AS `pe_ret_code`,`mr_pl_exec`.`pe_ret_state` AS `pe_ret_state`,`mr_pl_exec`.`pe_type` AS `pe_type`,`mr_pipeline`.`pl_oldid` AS `pl_oldid`,`mr_pipeline`.`pl_name` AS `pl_name`,`mr_pipeline`.`pl_desc` AS `pl_desc`,`mr_pipeline`.`pl_create_tm` AS `pl_create_tm`,`mr_pipeline`.`pl_modify_tm` AS `pl_modify_tm` from (`mr_pl_exec` join `mr_pipeline` on((`mr_pl_exec`.`pl_id` = `mr_pipeline`.`pl_id`))) group by `mr_pl_exec`.`pe_id`,`mr_pl_exec`.`pl_id` order by `mr_pl_exec`.`pe_id`,`mr_pl_exec`.`pl_id` ;

-- ----------------------------
-- View structure for `v_pln_exec`
-- ----------------------------
DROP VIEW IF EXISTS `v_pln_exec`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW `v_pln_exec` AS select `mr_pln_exec`.`pln_id` AS `pln_id`,`mr_pln_exec`.`pe_id` AS `pe_id`,`mr_pln_exec`.`pl_node` AS `pl_node`,`mr_pln_exec`.`pln_stm` AS `pln_stm`,`mr_pln_exec`.`pln_etm` AS `pln_etm`,`mr_pln_exec`.`pln_desc` AS `pln_desc`,`mr_pln_exec`.`pln_ret_code` AS `pln_ret_code`,`mr_pln_exec`.`pln_ret_state` AS `pln_ret_state`,`mr_pipeline`.`pl_name` AS `pl_name`,`mr_pipeline`.`pl_oldid` AS `pl_oldid`,`mr_pl_exec`.`pe_stm` AS `pe_stm`,`mr_pl_exec`.`pe_etm` AS `pe_etm`,`mr_pl_exec`.`pe_desc` AS `pe_desc`,`mr_pl_exec`.`pe_ret_code` AS `pe_ret_code`,`mr_pl_exec`.`pe_ret_state` AS `pe_ret_state`,`mr_pl_exec`.`pe_type` AS `pe_type`,`mr_pl_node`.`node_type` AS `node_type`,`mr_pl_node`.`ref_id` AS `ref_id`,`mr_pl_node`.`src_id` AS `src_id`,`mr_pl_node`.`trg_id` AS `trg_id`,`mr_pl_node`.`src_type` AS `src_type`,`mr_pl_node`.`trg_type` AS `trg_type`,`mr_pl_node`.`node_desc` AS `node_desc`,`mr_pl_node`.`create_tm` AS `create_tm`,`mr_pl_node`.`modify_tm` AS `modify_tm`,`mr_pipeline`.`pl_modify_tm` AS `pl_modify_tm`,`mr_pipeline`.`pl_id` AS `pl_id` from (((`mr_pln_exec` join `mr_pl_exec` on((`mr_pln_exec`.`pe_id` = `mr_pl_exec`.`pe_id`))) join `mr_pipeline` on((`mr_pl_exec`.`pl_id` = `mr_pipeline`.`pl_id`))) join `mr_pl_node` on(((`mr_pl_node`.`pl_id` = `mr_pipeline`.`pl_id`) and (`mr_pln_exec`.`pl_node` = `mr_pl_node`.`node_id`)))) group by `mr_pln_exec`.`pe_id`,`mr_pln_exec`.`pl_node`,`mr_pl_exec`.`pl_id` ;

-- ----------------------------
-- View structure for `v_pln_pl`
-- ----------------------------
DROP VIEW IF EXISTS `v_pln_pl`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW `v_pln_pl` AS select `node`.`node_id` AS `node_id`,`node`.`node_type` AS `node_type`,`node`.`pl_id` AS `pl_id`,`node`.`ref_id` AS `ref_id`,`node`.`src_id` AS `src_id`,`node`.`trg_id` AS `trg_id`,`node`.`src_type` AS `src_type`,`node`.`trg_type` AS `trg_type`,`node`.`node_desc` AS `node_desc`,`node`.`create_tm` AS `create_tm`,`node`.`modify_tm` AS `modify_tm`,`pl`.`pl_name` AS `pl_name`,`pl`.`pl_oldid` AS `pl_oldid`,`pl`.`pl_desc` AS `pl_desc`,`pl`.`pl_create_tm` AS `pl_create_tm`,`pl`.`pl_modify_tm` AS `pl_modify_tm`,`ref_pl`.`pl_name` AS `ref_pl_name`,`ref_pl`.`pl_oldid` AS `ref_pl_oldid`,`ref_pl`.`pl_desc` AS `ref_pl_desc`,`ref_pl`.`pl_create_tm` AS `ref_create_tm`,`ref_pl`.`pl_modify_tm` AS `ref_modify_tm` from ((`mr_pl_node` `node` join `mr_pipeline` `pl` on((`node`.`pl_id` = `pl`.`pl_id`))) join `mr_pipeline` `ref_pl` on(((`node`.`ref_id` = `ref_pl`.`pl_id`) and (`node`.`node_type` = '1')))) group by `node`.`node_id`,`node`.`ref_id`,`node`.`src_id`,`node`.`trg_id` order by `node`.`node_id`,`node`.`ref_id`,`node`.`src_id`,`node`.`trg_id` ;

-- ----------------------------
-- View structure for `v_pln_scpt`
-- ----------------------------
DROP VIEW IF EXISTS `v_pln_scpt`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW `v_pln_scpt` AS select `node`.`node_id` AS `node_id`,`node`.`node_type` AS `node_type`,`node`.`pl_id` AS `pl_id`,`node`.`ref_id` AS `ref_id`,`node`.`src_id` AS `src_id`,`node`.`trg_id` AS `trg_id`,`node`.`src_type` AS `src_type`,`node`.`trg_type` AS `trg_type`,`node`.`node_desc` AS `node_desc`,`node`.`create_tm` AS `create_tm`,`node`.`modify_tm` AS `modify_tm`,`pl`.`pl_name` AS `pl_name`,`pl`.`pl_oldid` AS `pl_oldid`,`pl`.`pl_desc` AS `pl_desc`,`pl`.`pl_create_tm` AS `pl_create_tm`,`pl`.`pl_modify_tm` AS `pl_modify_tm`,`ref_scpt`.`scpt_name` AS `ref_scpt_name`,`ref_scpt`.`host` AS `ref_scpt_host`,`ref_scpt`.`host_id` AS `trg_scpt_host_id`,`ref_scpt`.`ip` AS `ref_scpt_ip` from ((`mr_pl_node` `node` join `mr_pipeline` `pl` on((`node`.`pl_id` = `pl`.`pl_id`))) join `v_scpt_host` `ref_scpt` on(((`node`.`ref_id` = `ref_scpt`.`scpt_host_id`) and (`node`.`node_type` = '0')))) group by `node`.`node_id`,`node`.`ref_id`,`node`.`src_id`,`node`.`trg_id` order by `node`.`node_id`,`node`.`ref_id`,`node`.`src_id`,`node`.`trg_id` ;

-- ----------------------------
-- View structure for `v_scpt_host`
-- ----------------------------
DROP VIEW IF EXISTS `v_scpt_host`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW `v_scpt_host` AS select `mr_scpt_host`.`scpt_host_id` AS `scpt_host_id`,`mr_host`.`host_id` AS `host_id`,`mr_host`.`host` AS `host`,`mr_host`.`desc` AS `desc`,`mr_script`.`scpt_name` AS `scpt_name`,`mr_script`.`scpt_script` AS `scpt_script`,`mr_script`.`scpt_desc` AS `scpt_desc`,`mr_host`.`ip` AS `ip` from ((`mr_host` join `mr_scpt_host` on((`mr_scpt_host`.`host_id` = `mr_host`.`host_id`))) join `mr_script` on((`mr_scpt_host`.`scpt_id` = `mr_script`.`scpt_id`))) group by `mr_script`.`scpt_id`,`mr_host`.`host_id` order by `mr_script`.`scpt_id`,`mr_host`.`host_id` ;
