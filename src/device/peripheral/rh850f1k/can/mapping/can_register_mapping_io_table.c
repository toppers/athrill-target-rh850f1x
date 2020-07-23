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
#include "can_register.h"
#include "can_config.h"
#include "can_register_mapping_io_table.h"

CanRegisterMappingType can_reigster_mapping_table[CAN_REGISTER_MAPPING_TABLE_NUM] = {
		/*
		 * チャネル関連レジスタ
		 */
		{ (RSCANn_base + 0x0000), 4U, (16U * CAN_CHANNEL_NUM), can_register_mapping_io_CHANNEL },
		/*
		 * グローバル関連レジスタ
		 */
		{ (RSCANn_base + 0x0084), 4U, 0U, can_register_mapping_io_RSCANnGCFG },
		{ (RSCANn_base + 0x0088), 4U, 0U, can_register_mapping_io_RSCANnGCTR },
		{ (RSCANn_base + 0x008C), 4U, 0U, can_register_mapping_io_RSCANnGSTS },
		{ (RSCANn_base + 0x0090), 4U, 0U, can_register_mapping_io_RSCANnGERFL },
		{ (RSCANn_base + 0x0460), 4U, 0U, can_register_mapping_io_RSCANnGTINTSTS0 },
		{ (RSCANn_base + 0x0464), 4U, 0U, can_register_mapping_io_RSCANnGTINTSTS1 },

		/*
		 * 受信ルール関連レジスタ
		 */
		{ (RSCANn_base + 0x0098), 4U, 0U, can_register_mapping_io_RSCANnGAFLECTR },
		{ (RSCANn_base + 0x009C), 4U, 0U, can_register_mapping_io_RSCANnGAFLCFG0 },
		{ (RSCANn_base + 0x00A0), 4U, 0U, can_register_mapping_io_RSCANnGAFLCFG1 },
		{ (RSCANn_base + 0x0500), 4U, (16U * CAN_RX_PAGE_RULE_NUM), can_register_mapping_io_RX_RULE },

		/*
		 * 受信バッファ関連レジスタ
		 */
		{ (RSCANn_base + 0x00A4), 4U, 0U, can_register_mapping_io_RSCANnRMNB },
		{ (RSCANn_base + 0x00A8), 4U, (4U * CAN_RX_MSGIN_FLAG_NUM), can_register_mapping_io_RSCANnRMNDy },
		{ (RSCANn_base + 0x0600), 4U, (16U * CAN_RX_BUFFER_NUM), can_register_mapping_io_RX_BUFFER },

		/*
		 * 受信 FIFO バッファ関連レジスタ
		 */
		{ (RSCANn_base + 0x00B8), 4U, (4U * CAN_RX_FIFO_NUM), can_register_mapping_io_RSCANnRFCCx },
		{ (RSCANn_base + 0x00D8), 4U, (4U * CAN_RX_FIFO_NUM), can_register_mapping_io_RSCANnRFSTSx },
		{ (RSCANn_base + 0x00F8), 4U, (4U * CAN_RX_FIFO_NUM), can_register_mapping_io_RSCANnRFPCTRx },
		{ (RSCANn_base + 0x0E00), 4U, (16U * CAN_RX_FIFO_NUM), can_register_mapping_io_RX_FIFO },

		/*
		 * 送受信 FIFO バッファ関連レジスタ
		 */
		{ (RSCANn_base + 0x0118), 4U, (4U * (CAN_CHANNEL_NUM * CAN_TXRX_CHANNEL_FIFO_NUM)), can_register_mapping_io_RSCANnCFCCk },
		{ (RSCANn_base + 0x0178), 4U, (4U * (CAN_CHANNEL_NUM * CAN_TXRX_CHANNEL_FIFO_NUM)), can_register_mapping_io_RSCANnCFSTSk },
		{ (RSCANn_base + 0x01D8), 4U, (4U * (CAN_CHANNEL_NUM * CAN_TXRX_CHANNEL_FIFO_NUM)), can_register_mapping_io_RSCANnCFPCTRk },
		{ (RSCANn_base + 0x0E80), 4U, (16U * (CAN_CHANNEL_NUM * CAN_TXRX_CHANNEL_FIFO_NUM)), can_register_mapping_io_TXRX_FIFO },

		/*
		 * FIFO ステータス関連レジスタ
		 */
		{ (RSCANn_base + 0x0238), 4U, 0U, can_register_mapping_io_RSCANnFESTS },
		{ (RSCANn_base + 0x023C), 4U, 0U, can_register_mapping_io_RSCANnFFSTS },
		{ (RSCANn_base + 0x0240), 4U, 0U, can_register_mapping_io_RSCANnFMSTS },
		{ (RSCANn_base + 0x0244), 4U, 0U, can_register_mapping_io_RSCANnRFISTS },
		{ (RSCANn_base + 0x0248), 4U, 0U, can_register_mapping_io_RSCANnCFRISTS },
		{ (RSCANn_base + 0x024C), 4U, 0U, can_register_mapping_io_RSCANnCFTISTS },

		/*
		 * 送信バッファ関連レジスタ
		 */
		{ (RSCANn_base + 0x0250), 1U, (1U * (CAN_TX_BUFFER_NUM)), can_register_mapping_io_RSCANnTMCp },
		{ (RSCANn_base + 0x02D0), 1U, (1U * (CAN_TX_BUFFER_NUM)), can_register_mapping_io_RSCANnTMSTSp },
		{ (RSCANn_base + 0x1000), 4U, (16U * CAN_TX_BUFFER_NUM), can_register_mapping_io_TX_BUFFER },
		{ (RSCANn_base + 0x0390), 4U, (4U * CAN_TX_BUFFER_FLAG_NUM), can_register_mapping_io_RSCANnTMIECy },

		/*
		 * 送信バッファステータス関連レジスタ
		 */
		{ (RSCANn_base + 0x0350), 4U, (4U * CAN_TX_BUFFER_FLAG_NUM), can_register_mapping_io_RSCANnTMTRSTSy },
		{ (RSCANn_base + 0x0370), 4U, (4U * CAN_TX_BUFFER_FLAG_NUM), can_register_mapping_io_RSCANnTMTCSTSy },

		/*
		 * 送信キュー関連レジスタ
		 */
		{ (RSCANn_base + 0x03A0), 4U, (4U * CAN_CHANNEL_NUM), can_register_mapping_io_RSCANnTXQCCm },
		{ (RSCANn_base + 0x03C0), 4U, (4U * CAN_CHANNEL_NUM), can_register_mapping_io_RSCANnTXQSTSm },
		{ (RSCANn_base + 0x03E0), 4U, (4U * CAN_CHANNEL_NUM), can_register_mapping_io_RSCANnTXQPCTRm },

		/*
		 * 送信履歴関連レジスタ
		 */
		{ (RSCANn_base + 0x0400), 4U, (4U * CAN_CHANNEL_NUM), can_register_mapping_io_RSCANnTHLCCm },
		{ (RSCANn_base + 0x0420), 4U, (4U * CAN_CHANNEL_NUM), can_register_mapping_io_RSCANnTHLSTSm },
		{ (RSCANn_base + 0x0440), 4U, (4U * CAN_CHANNEL_NUM), can_register_mapping_io_RSCANnTHLPCTRm },
		{ (RSCANn_base + 0x1800), 4U, (4U * CAN_CHANNEL_NUM), can_register_mapping_io_RSCANnTHLACCm },
};
