/*
 *  TOPPERS athrill
 *     Athrill is a CPU emulator. Athrill was developed to easily
 *     execute and debug embedded programs on virtual microcomputer.
 *
 *  Copyright (C) 2019 by Center for Embedded Computing Systems
 *                          Graduate School of Informatics, Nagoya Univ., JAPAN
 *  Copyright (C) 2019 by ESM, Inc.
 *
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id$
 */
#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_

#define CAN_CHANNEL_NUM				6U


#define CAN_RX_UNIT_RULE_MAX		( 64U * CAN_CHANNEL_NUM )
#define CAN_RX_CHANNEL_RULE_MAX		128U

#define CAN_RX_PAGE_RULE_NUM		16U
#define CAN_RX_RULE_TABLE_PAGE_NUM	24U


#define CAN_RX_BUFFER_NUM			96U
#define CAN_RX_BUFFER_FLAG_BIT_NUM  32U

#define CAN_RX_MSGIN_FLAG_NUM		3U

#define CAN_RX_BUFFER_DATA_SIZE		8U
#define CAN_TX_BUFFER_DATA_SIZE		8U


#define CAN_RX_FIFO_QUEUE_MAX		128U
#define CAN_RX_FIFO_NUM				8U

#define CAN_TX_BUFFER_FLAG_NUM		3U
#define CAN_TX_BUFFER_FLAG_BIT_NUM	32U
#define CAN_TX_CHANNEL_BUFFER_NUM	16U
#define CAN_TX_FIFO_QUEUE_MAX		128U
#define CAN_TX_BUFFER_NUM			96U

#define CAN_TXRX_CHANNEL_FIFO_NUM	3U
#define CAN_FIFO_FLAG_NUM			3U


#define CAN_TX_HISTORY_BUFFER_NUM	16U
#define CAN_TX_HISTORY_INTR_BUFNUM	12U

#endif /* _CAN_CONFIG_H_ */
