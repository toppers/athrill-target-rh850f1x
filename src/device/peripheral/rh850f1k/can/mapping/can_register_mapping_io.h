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
#include "can_register_mapping.h"


/*
 * チャネル関連レジスタ
 */
extern void can_register_mapping_io_CHANNEL(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_RSCANnCmCFG(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCmCTR(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCmSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCmERFL(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * グローバル関連レジスタ
 */
extern void can_register_mapping_io_RSCANnGCFG(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGCTR(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGERFL(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGTINTSTS0(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGTINTSTS1(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * 受信ルール関連レジスタ
 */
extern void can_register_mapping_io_RSCANnGAFLECTR(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGAFLCFG0(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGAFLCFG1(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_RX_RULE(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGAFLIDj(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGAFLMj(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGAFLP0j(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnGAFLP1j(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * 受信バッファ関連レジスタ
 */
extern void can_register_mapping_io_RSCANnRMNB(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRMNDy(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_RX_BUFFER(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_RSCANnRMIDq(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRMPTRq(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRMDF0q(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRMDF1q(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * 受信 FIFO バッファ関連レジスタ
 */
extern void can_register_mapping_io_RSCANnRFCCx(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRFSTSx(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRFPCTRx(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_RX_FIFO(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_RSCANnRFIDx(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRFPTRx(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRFDF0x(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRFDF1x(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * 送受信 FIFO バッファ関連レジスタ
 */
extern void can_register_mapping_io_RSCANnCFCCk(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFSTSk(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFPCTRk(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_TXRX_FIFO(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFIDk(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFPTRk(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFDF0k(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFDF1k(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * FIFO ステータス関連レジスタ
 */
extern void can_register_mapping_io_RSCANnFESTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnFFSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnFMSTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnRFISTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFRISTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnCFTISTS(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * 送信バッファ関連レジスタ
 */
extern void can_register_mapping_io_RSCANnTMCp(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTMSTSp(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_TX_BUFFER(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

extern void can_register_mapping_io_RSCANnTMIDp(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTMPTRp(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTMDF0p(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTMDF1p(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTMIECy(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * 送信バッファステータス関連レジスタ
 */
extern void can_register_mapping_io_RSCANnTMTRSTSy(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTMTCSTSy(CanRegisterIoType io_type, CanRegisterIoArgType *arg);


/*
 * 送信キュー関連レジスタ
 */
extern void can_register_mapping_io_RSCANnTXQCCm(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTXQSTSm(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTXQPCTRm(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

/*
 * 送信履歴関連レジスタ
 */
extern void can_register_mapping_io_RSCANnTHLCCm(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTHLSTSm(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTHLPCTRm(CanRegisterIoType io_type, CanRegisterIoArgType *arg);
extern void can_register_mapping_io_RSCANnTHLACCm(CanRegisterIoType io_type, CanRegisterIoArgType *arg);

