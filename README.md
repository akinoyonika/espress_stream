# espress_stream - ストリーム通信用ライブラリー

## 概要

本ソフトウェアはデータグラム通信上でストリーム通信を実現するためのライブラリーです。
認証付き暗号化パケットによりメッセージを秘匿します。

本ソフトウェアのソースコードはC言語によって記述されています。基本部分はアーキテクチャー依存部を分離しており、組み込みシステムを含む各種プラットフォーム、OS上で動作させることができるようデザインされています。
スレッド、ヒープの未使用を選択することも可能です。

本リポジトリではlinuxに対するポーティングが提供されています。
linuxに対するポーティングではデータグラム通信にUDPを用いています。

主な特徴を以下に示します。

接続
: 1RTT接続

切断
: 1/2RTTハーフクローズ

死活監視
: 無受信期間タイムアウトによる切断検出および無送信データ期間のビーコン送信による活線維持

フロー制御
: スライディングウィンドウ方式

輻輳制御
: New Reno

鍵交換
: ECDH (Curve25519) [RFC 7748]

鍵拡張
: HKDF (SHA2-256) [RFC 5869]

暗号
: AES256-CTR

メッセージ認証符号
: HMAC-SHA2-256-ETM

## パッケージ

本ソフトウェアはソースパッケージにて提供しています。以下をダウンロードしてください。

- [espress_stream.20240110.tar.gz](http://akino-yo.blue.coocan.jp/akinoyonika/espress_stream/espress_stream.20240110.tar.gz)

<details><summary>変更履歴</summary>

espress_stream.20240110.tar.gz (1.0-0)
: - APIの<tt>espress_stream_wait_for_any</tt>と<tt>espress_stream_poll</tt>のプロトタイプを変更。機能を拡充した。
 - APIの仕様変更のため、メジャーバージョン番号を変え、マイナーバージョン番号をリセットした。
 - サンプルアプリケーション<tt>es_ftx</tt>をAPI変更に対応させた。

espress_stream.20231228.tar.gz (0.9-0)
: - 一部設定値を固定値から初期値付き変数に変更。
 - いくつかのモジュールに関して、初期化時のエラーチェックを強化した。
 - サンプルアプリケーション<tt>es_ftx</tt>において、サーバー公開鍵を指定しない場合、サーバー認証を行わないようにした。
 - サンプルアプリケーション<tt>es_frx</tt>において、クライアントからのサーバー認証メッセージがない場合、サーバー認証応答もしないようにした。
 - サンプルアプリケーションで使用する署名アルゴリズムの実装において固定テーブルの一部が変数になっていたのを固定値に修正。
 - 多ビット長整数演算部のローカル変数が外部参照可能になっていたのを外部参照不可に修正。
 - サンプルアプリケーション<tt>es_ftx</tt>において、データの送信タイミングが遅れる可能性があったため、これを修正。
 - シングルスレッド動作設定時、<tt>espress_stream_poll</tt>内で少なくとも1度は一連のyield処理が呼び出されるようにした。

espress_stream.20231221.tar.gz (0.8-0)
: - 好ましくない関数名の修正、未削除のコメントアウトコードの削除、テストコードのバグ修正等、動作に影響のないソースコードの修正。

espress_stream.20231219.tar.gz (0.8-0)
: - 多ビット長整数演算部の改良。

espress_stream.20231214.tar.gz (0.7-0)
: - 再送タイムアウトにlinuxと同様の最小値を設定した。
 - リファクタリング、コメントの修正。

espress_stream.20231209.tar.gz (0.6-0)
: - デスティネーションのネットワークアドレスをパケット受信毎に更新するようにした。
 - 現在のソース及びデスティネーションのネットワークアドレスを取得する機能を追加。
 - サンプルアプリケーション<tt>es_ftx</tt>において、プログレス表示の更新頻度を上げた。
 - データグラム通信とのインターフェイスを改良した。

espress_stream.20231202.tar.gz (0.5-0)
: - 送受信バッファーの使用量を取得する機能を追加。
 - サンプルアプリケーション<tt>es_ftx</tt>において、送信完了タイムアウト検出時に送信バッファーの使用量の変化を考慮するようにし、タイムアウトしにくくした。
 - サンプルアプリケーション<tt>es_ftx</tt>において、32ビット環境でも64ビット以上の整数を保証するため、<tt>unsigned long</tt>を<tt>uint64_t</tt>に変更した。
 - linux用の各種設定値を調整。
 - シングルスレッド動作設定時、アプリケーションのマルチスレッド動作に対して必要な排他制御の一部が欠落していたので、その部分を追加、修正。

espress_stream.20231125.tar.gz (0.4-0)
: - サンプルアプリケーション<tt>es_frx</tt>作成時に用いる<tt>mk_cert_h.sh</tt>を改良。

espress_stream.20231124.tar.gz (0.4-0)
: - 多ビット長整数定義で64ビットの定数定義にあったバグを修正。
 - サンプルアプリケーションの認証機能の移植部を内製化。

espress_stream.20231117.tar.gz (0.3-0)
: - リトライタイムアウト値の上限をセッションタイムアウト値と同じ値に設定。
 - 異なるセッションのイベントの発生を同時に待つ機能を追加。

espress_stream.20231107.tar.gz (0.2-0)
: - APIレイヤーで、セッションリソースがスタティックでかつシングルスレッドモード時のセッションリソース列挙にバグあり。これを修正。
 - linux用の依存定義に対して32ビットコードモデルに適した定義を追加。32ビットコードモデルでのコンパイル時にその定義を使用するようにした。

espress_stream.20231106.tar.gz (0.1-0)
: - READMEの僅かな修正のみ。

espress_stream.20231102.tar.gz (0.1-0)
: - サンプルアプリケーションに認証機能追加。
 - 多ビット長整数演算の割り算にあったバグを修正。

espress_stream.20231023.tar.gz (0.0-0)
: - 初版

</details>

## 導入

### 準備

本ソフトウェアのlinuxポーティングでは、基本的なgcc、make等が使えるlinux環境に対し導入できます。
本ソフトウェアのlinuxポーティングの導入、動作の確認はubuntuで行われています。
ubuntuを使用する場合、build-essentialパッケージの導入を行えば、本ソフトウェアの導入、動作が可能になります。

最初に適当なディレクトリでパッケージの解凍を行っておいてください。

```
$ tar zxvf espress_stream.########.tar.gz
```

以下に導入のための手順を示します。

### ライブラリー

```
$ cd espress_stream/arch/linux
$ make clean
$ make
$ sudo make uninstall
$ sudo make install
$ make clean
```

### サンプルアプリケーション

```
$ cd espress_stream/arch/linux/app/es_ftx
$ make clean
$ make
$ sudo make uninstall
$ sudo make install
$ make clean
$ cd ../es_frx
$ ./mk_cert_h.sh
1) user id: foo
1) user password: 
2) user id: bar
2) user password: 
3) user id: 
$ make clean
$ make
$ sudo make uninstall
$ sudo make install
$ make clean
```

サンプルアプリケーションの<tt>ex_frx</tt>はサーバーアプリケーションです。
これに必要なサーバー認証情報およびクライアント認証情報は実行コードに埋め込まれます。
<tt>mk_cert_h.sh</tt>はこれらの情報を定義した<tt>ex_frx</tt>用のソースコードの一部(<tt>cert.h</tt>)を生成します。
<tt>mk_cert_h.sh</tt>を実行するとユーザーIDとパスワードの入力を求められるので、入力してください。
ユーザー情報は複数設定できるので、繰り返し入力してください。
空行の入力で終了します。

## 使用方法

### ライブラリー (<tt>libespress_stream.so</tt>)

本ソフトウェアのlinuxポーティングでは、ライブラリーは共有ライブラリーとして<tt>/usr/local/lib</tt>に<tt>libespress_stream.so</tt>の名前で導入されます。ヘッダーファイルは<tt>/usr/local/include</tt>に<tt>espress_stream/api.h</tt>で導入されます。
アプリケーション作成時にこれらをインクルード、リンクするようにしてください。
例えば、ヘッダーファイルのインクルードはソースファイル中で以下のように行います。

```
#include "espress_stream/api.h"
```

また、例えば、ライブラリーのリンクは以下のように行います。

```
$ gcc main.c -lpthread -lespress_stream
```

ライブラリーAPIの仕様は<tt>api/include/espress_stream/api.h</tt>にコードドキュメントとして示されていますので、そちらを参照ください。

<details><summary><code>api/include/espress_stream/api.h</code></summary>

```
#ifndef __ESPRESS_STREAM_API_H__
#define __ESPRESS_STREAM_API_H__

/* espress_stream APIs
 * 
 * - Function calling procedure.
 * 
 *     espress_stream_begin
 *        |
 *        |<--------------------------------------------------------------+
 *        |                                                               |
 *        +-------------------------------+...                            |
 *        |                               |                               |
 *        V                               V                               |
 *     espress_stream_begin_interface  espress_stream_begin_interface     |
 *        |                               :                               |
 *        |<----------------------------------------------------------+   |
 *        |                                                           |   |
 *        +-------------------------------+...                        |   |
 *        |(client session)               |(server session)           |   |
 *        V                               V                           |   |
 *     espress_stream_begin_session    espress_stream_begin_session   |   |
 *        |                               |                           |   |
 *     espress_stream_connect          espress_stream_connect_detect  |   |
 *        |                               |                           |   |
 *     espress_stream_connect_result      |                           |   |
 *        |                               |                           |   |
 *        |<------------------------------+                           |   |
 *        |                                                           |   |
 *        |<------------------------------+                           |   |
 *        |                               |                           |   |
 *        V                               |                           |   |
 *     espress_stream_send                |                           |   |
 *     espress_stream_flush               |                           |   |
 *     espress_stream_recv                |                           |   |
 *        |                               |                           |   |
 *        +-------------------------------+                           |   |
 *        |                                                           |   |
 *        V                                                           |   |
 *     espress_stream_disconnect                                      |   |
 *        |                                                           |   |
 *        V                                                           |   |
 *     espress_stream_disconnect_result                               |   |
 *        |                                                           |   |
 *        V                                                           |   |
 *     espress_stream_end_session                                     |   |
 *        |                                                           |   |
 *        +-----------------------------------------------------------+   |
 *        |                                                               |
 *        V                                                               |
 *     espress_stream_end_interface                                       |
 *        |                                                               |
 *        +---------------------------------------------------------------+
 *        |
 *        V
 *     espress_stream_end
 * 
 * - Async running.
 * 
 *     Different sessions are async runnable.
 *     In single session functions are not able to async run
 *     except for "espress_stream_wait_break" function.
 * 
 * - Return code.
 * 
 *     - == 0
 *         Complete in success.
 * 
 *     - > 0
 *         Finished in failure.
 *         In a session when a function return positive value in spite of right procedure,
 *         this session should be destroyed by "espress_stream_end_session" function.
 *         Reprocess from beginning session.
 * 
 *     - == -1
 *         Unfinished. Functions with timeout may return negative one.
 *         Retry run this function.
 */

#include "espress_stream/api_depend.h"

#define ESPRESS_STREAM_SESSION_HASH_SIZE (32)
#define ESPRESS_STREAM_MAX_NUM_POLL_SESSION (ESPRESS_STREAM_MAX_NUM_SESSION)

#define ESPRESS_STREAM_EVENT_CONNECT_RESULT (1 << 0)
#define ESPRESS_STREAM_EVENT_CONNECT_DETECT (1 << 1)
#define ESPRESS_STREAM_EVENT_DISCONNECT_RESULT (1 << 2)
#define ESPRESS_STREAM_EVENT_SEND (1 << 3)
#define ESPRESS_STREAM_EVENT_RECV (1 << 4)

struct espress_stream_session_ex_param {
    int (*send_connect_payload)(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param);
    void *send_connect_payload_param;
    int (*recv_connect_payload)(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param);
    void *recv_connect_payload_param;
    int (*send_disconnect_payload)(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size), void *param);
    void *send_disconnect_payload_param;
    int (*recv_disconnect_payload)(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param);
    void *recv_disconnect_payload_param;
};

/** @brief Begin stack.
 * 
 * Begin espress_stream protocol stack.
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_begin(void);

/** @brief Finish stack.
 * 
 * Finish espress_stream protocol stack.
 */
void espress_stream_end(void);

/** @brief Get copyright string.
 * 
 * @return
 *     Stored address of null terminated copyright string.
 */
char const *espress_stream_copyright(void);

/** @brief Tick time.
 * 
 * Get tick time in microsecond.
 * 
 * @return
 *     Tick time. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_time(void);

/** @brief Begin interface.
 * 
 * Begin interface. It is consists of a datagrum session (UDP socket when on linux).
 * 
 * @param[out] ifd
 *     Store address of interface ID (It's type is void *).
 * @param[in] address
 *     Stored address of an interface bind network address (include port number when it is IP address).
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_begin_interface(void **ifd, struct sockaddr_storage const *address);

/** @brief Finish interface.
 * 
 * Finish interface.
 * 
 * @param[in] ifd
 *     Interface ID.
 */
void espress_stream_end_interface(void *ifd);

/** @brief Begin session.
 * 
 * Begin session. It is consists of a one connection.
 * In case of server session, the connection established session is used for transmission as it is.
 * When multiple connection acception in same time is required, begin multiple server session in same time.
 * 
 * @param[out] sid
 *     Store address of session ID (It's type is void *).
 * @param[in] ifd
 *     An interface ID that the session belong.
 * @param[in] is_server
 *     When this session is a server session, set it to non zero value.
 * @param[in] ex_param
 *     For future use. set NULL yet.
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_begin_session(void **sid, void *ifd, int is_server, struct espress_stream_session_ex_param const *ex_param);

/** @brief Finish session.
 * 
 * Finish session.
 * 
 * @param[in] sid
 *     Session ID.
 */
void espress_stream_end_session(void *sid);

/** @brief Request to connect.
 * 
 * Request to connect (only for client session).
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] address
 *     Stored address of a destination network address (include port number when it is IP address).
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_connect(void *sid, struct sockaddr_storage const *address);

/** @brief Confirm result of request to connect.
 * 
 * Confirm result of request to connect. It may be waiting for result.
 * It may be not able to get result ever. In this case user should rise timeout error.
 * This is used for only client.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for result yet.
 *     > 0  : Failure.
 */
int espress_stream_connect_result(void *sid, unsigned timeout);

/** @brief Confirm detection of incoming connect.
 * 
 * Confirm detection of incoming connect. It may be waiting for detection.
 * It may be not able to get detection ever. In this case user should rise timeout error.
 * This is used for only server.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for detection yet.
 *     > 0  : Failure.
 */
int espress_stream_connect_detect(void *sid, unsigned timeout);

/** @brief Request to disconnect.
 * 
 * Request to disconnect. The disconnect is to close the sending part of half session.
 * The receiving part of the other half session is able to active close by only the destination node.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_disconnect(void *sid);

/** @brief Confirm result of request to disconnect.
 * 
 * Confirm result of request to disconnect. It may be waiting for result.
 * It may be not able to get result ever. In this case user should rise timeout error.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for result yet.
 *     > 0  : Failure.
 */
int espress_stream_disconnect_result(void *sid, unsigned timeout);

/** @brief Request to send data.
 * 
 * Request to send data. The all send request data may be not able to be accepted by timeout time.
 * When accepted send request data is nothing, the sent data size do not become zero but return value become negative one.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] data
 *     Stored address of data.
 * @param[in,out] data_size
 *     Stored address of data size and store address of accepted send request data size.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for acceptable state yet.
 *     > 0  : Failure.
 */
int espress_stream_send(void *sid, void const *data, unsigned *data_size, unsigned timeout);

/** @brief Request to flush data.
 * 
 * Request to flush data. When send request data is accepted,
 * the last remainder part of the data divided by packet payload size are not sent soon.
 * Requesting to flush is to force send this data.
 * When this function's return value is negative one,
 * this data is not sent yet but it would be sent as soon as possible.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for send complete yet.
 *     > 0  : Failure.
 */
int espress_stream_flush(void *sid, unsigned timeout);

/** @brief Request to receive data.
 * 
 * Request to receive data. When received data is nothing until timeout time,
 * the received data size do not become zero but return value become negative one.
 * When this function's return value is zero and received data size is zero,
 * it is mean that destination node's disconnect is detected.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] data
 *     Store address of received data.
 * @param[in,out] data_size
 *     Stored address of data region size and store address of received data size.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for incoming data yet.
 *     > 0  : Failure.
 */
int espress_stream_recv(void *sid, void *data, unsigned *data_size, unsigned timeout);

/** @brief Wait for any event.
 * 
 * Wait for gettable result for the any function with timeout parameter.
 * To confirm which function is able to get result, see return value of each function called with zero timeout.
 * When this function finish in success, but none function may be able to get result.
 * In this case, repeat call this function.
 * You are able to choose event types which relate the function to wait for result.
 * 
 * | Event types                              | Functions                                 |
 * |------------------------------------------|-------------------------------------------|
 * | ESPRESS_STREAM_EVENT_CONNECT_RESULT      | espress_stream_connect_result             |
 * | ESPRESS_STREAM_EVENT_CONNECT_DETECT      | espress_stream_connect_detect             |
 * | ESPRESS_STREAM_EVENT_DISCONNECT_RESULT   | espress_stream_disconnect_result          |
 * | ESPRESS_STREAM_EVENT_SEND                | espress_stream_send, espress_stream_flush |
 * | ESPRESS_STREAM_EVENT_RECV                | espress_stream_recv                       |
 * 
 * One or more event types are selectable. Set concated event types with "|" to the event parameter.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] event
 *     Event types.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for event yet.
 *     > 0  : Failure.
 */
int espress_stream_wait_any(void *sid, unsigned event, unsigned timeout);

/** @brief Break to waiting function
 * 
 * Break to waiting function. Apply this function to the function with timeout parameter.
 * The waiting function return soon because of timeout, when this function is called.
 * 
 * @param[in] sid
 *     Session ID.
 */
void espress_stream_wait_break(void *sid);

/** @brief Polling multiple session.
 * 
 * Detect that any espress_stream_wait_any function result for one or more sids will be success or failure at least one.
 * 
 * @param[in] sids
 *     Session IDs array.
 * @param[in] events
 *     Event types array.
 * @param[in] num
 *     Number of sids and events items.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     >= 0 : Number of detected sids. (Zero indicates timeout.)
 *     == -1: Failure.
 */
int espress_stream_poll(void * const *sids, unsigned const *events, unsigned num, unsigned timeout);

/** @brief Get source address.
 * 
 * Get source network address.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] address
 *     Store address of a source network address.
 * 
 */
void espress_stream_src_address(void *sid, struct sockaddr_storage *address);

/** @brief Get destination address.
 * 
 * Get destination network address.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] address
 *     Store address of a destination network address.
 * 
 */
void espress_stream_dst_address(void *sid, struct sockaddr_storage *address);

/** @brief Get connection ID.
 * 
 * Get connection ID value.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Connection ID value. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_connection_id(void *sid);

/** @brief Get session HASH.
 * 
 * Get session HASH.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] session_hash
 *     Store address of session hash bytes. Its region size is ESPRESS_STREAM_SESSION_HASH_SIZE bytes at least.
 */
void espress_stream_session_hash(void *sid, void *session_hash);

/** @brief Get buffered send data size.
 * 
 * Get buffered send data size.
 * 
 * @param[out] max_size
 *     Store address of maximum buffered send data size. If retrieving it is't needed, set this parameter to NULL.
 * 
 * @return
 *     Buffered send data size.
 */
unsigned espress_stream_buffered_send_size(void *sid, unsigned *max_size);

/** @brief Get buffered recv data size.
 * 
 * Get buffered recv data size.
 * 
 * @param[out] max_size
 *     Store address of maximum buffered recv data size. If retrieving it is't needed, set this parameter to NULL.
 * 
 * @return
 *     Buffered recv data size.
 */
unsigned espress_stream_buffered_recv_size(void *sid, unsigned *max_size);

/** @brief Get rtt.
 * 
 * Get round trip time value. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Round trip time value. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_rtt(void *sid);

/** @brief Get rto.
 * 
 * Get retransmission timeout value. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Retransmission timeout value. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_rto(void *sid);

/** @brief Get dac.
 * 
 * Get duplicate acknowledge count. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Duplicate acknowledge count.
 */
unsigned espress_stream_dac(void *sid);

/** @brief Get cwnd.
 * 
 * Get congestion window size. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Congestion window size. The unit is packet payload size.
 */
unsigned espress_stream_cwnd(void *sid);

/** @brief Get send timeout count.
 * 
 * Get send timeout count. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Send timeout count.
 */
unsigned espress_stream_send_timeout_count(void *sid);

/** @brief Get fast retransmit count.
 * 
 * Get fast retransmit count. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Fast retransmit count.
 */
unsigned espress_stream_fast_retransmit_count(void *sid);

#endif /* __ESPRESS_STREAM_API_H__ */
```

</details>

使用方法の具体例はサンプルアプリケーションのソースコードを参照してください。
クライアントは<tt>arch/linux/app/es_ftx/src/main.c</tt>でサーバーは<tt>arch/linux/app/es_frx/src/main.c</tt>となります。

<details><summary><code>arch/linux/app/es_ftx/src/main.c</code></summary>

```
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <endian.h>
#include <signal.h>
#include <unistd.h>
#include <sys/random.h>

#include "espress_stream/api.h"
#include "cipher_api.h"

#define IS_DEBUGGING (0)
#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (4 *1024)
#define CONNECT_MESSAGE_SIZE (64)
#define USER_ID_SIZE (256)
#define USER_PASSWORD_SIZE (256)

static void *ifd;
static const char *arg_address;
static const char *arg_port;
static const char **arg_file_name;
static unsigned arg_num_file;
static unsigned char *arg_public_key;
static unsigned char _arg_public_key[CIPHER_ED25519_PUBLIC_KEY_SIZE];
static char const *arg_user_id;
static char const *arg_user_password;

static int inet_naton(char const *name, char const *port, struct sockaddr *address)
{
    int ret;
    struct addrinfo hint[1] = {0};
    struct addrinfo *addrinfo;
    hint->ai_family = AF_INET;
    hint->ai_socktype = SOCK_DGRAM;
    if (name == NULL) {
        hint->ai_flags = AI_PASSIVE;
    }
    ret = getaddrinfo(name, port, hint, &addrinfo);
    if (ret == 0) {
        memcpy(address, addrinfo->ai_addr, addrinfo->ai_addrlen);
        freeaddrinfo(addrinfo);
    }
    return ret;
}

static int scan_arg(int argc, char const **argv)
{
    int i;
    int j;
    int k;
    char const *p;
    int ret;
    char c = '\0';

    arg_address = "localhost";
    arg_port = "12345";
    arg_file_name = NULL;
    arg_num_file = 0;
    arg_public_key = NULL;
    arg_user_id = NULL;
    arg_user_password = NULL;

    for (i = 1 ; i < argc ; i++) {
        if (argv[i][0] == '-') {
            c = argv[i][1];
            switch (c) {
            case 'a':
            case 'p':
            case 'k':
            case 'i':
            case 'w':
                break;
            default:
                c = '\0';
                break;
            }
            continue;
        }
        switch (c) {
        case 'a':
            arg_address = argv[i];
            c = '\0';
            break;
        case 'p':
            arg_port = argv[i];
            c = '\0';
            break;
        case 'k':
            for (j = 0 ; (j < (CIPHER_ED25519_PUBLIC_KEY_SIZE * 2)) && (argv[i][j] != '\0') ; j++) {
                if (('0' <= argv[i][j]) && (argv[i][j] <= '9')) {
                    k = argv[i][j] - '0';
                } else if (('a' <= argv[i][j]) && (argv[i][j] <= 'f')) {
                    k = argv[i][j] - 'a' + 10;
                } else if (('A' <= argv[i][j]) && (argv[i][j] <= 'F')) {
                    k = argv[i][j] - 'A' + 10;
                } else {
                    break;
                }
                if ((j & 1) == 0) {
                    _arg_public_key[j >> 1] = (k << 4) & 0xf0;
                } else {
                    _arg_public_key[j >> 1] |= k & 0x0f;
                }
            }
            if (j == (CIPHER_ED25519_PUBLIC_KEY_SIZE * 2)) {
                arg_public_key = _arg_public_key;
            } else {
                arg_public_key = NULL;
            }
            c = '\0';
            break;
        case 'i':
            arg_user_id = argv[i];
            c = '\0';
            break;
        case 'w':
            arg_user_password = argv[i];
            c = '\0';
            break;
        default:
            if (arg_num_file == 0) {
                arg_file_name = argv + i;
                arg_num_file = 1;
                ret = 0;
            } else if ((arg_file_name + arg_num_file) == (argv + i)) {
                arg_num_file++;
                ret = 0;
            } else {
                ret = 1;
            }
            break;
        }
        if (ret) {
            break;
        }
    }

    ret = ret || (arg_num_file == 0);

    if (ret) {
        p = strrchr(argv[0], '/');
        if (p == NULL) {
            p = argv[0];
        } else {
            p++;
        }
        printf("%s "
                "[-a <destination address>] "
                "[-p <destination port>] "
                "[-k <destination public key>] "
                "[-i <user id>] "
                "[-w <user password>] "
                "<file name> [<file name> [...]]\n",
            p);
        printf("%s (C) 2023 akinoyonika\n", p);
        printf("%s\n", espress_stream_copyright());
    }

    return ret;
}

struct file_tx_attr {
    unsigned index;
    int is_attached;
    pthread_t thread;
    void *sid;
    int is_running;
    struct sockaddr_storage const *address;
    char const *file_name;
    int ret;
    unsigned char message[CONNECT_MESSAGE_SIZE];
    unsigned message_size;
};

static struct file_tx_attr *file_tx_attr = NULL;

#if IS_DEBUGGING
    static void disp(unsigned index, char const *main_file_name, uint64_t file_size, uint64_t progress, int fin_state,
        unsigned bufdsz, unsigned rtt, unsigned rto, unsigned dac, unsigned cwnd, unsigned send_timeout_count, unsigned fast_retransmit_count)
    {
        unsigned u;
        static pthread_mutex_t disp_mutex[1] = {PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP};
#       define FILE_NAME_DISP_WIDTH (24)

        pthread_mutex_lock(disp_mutex);

        for (u = 0 ; u < index ; u++) {
            putchar('\n');
            putchar('\n');
        }

        if (main_file_name == NULL) {
            printf("------------------------\r");
        }
        else if (FILE_NAME_DISP_WIDTH < strlen(main_file_name)) {
            printf("%.*s...\r", FILE_NAME_DISP_WIDTH - 3, main_file_name);
        } else {
            printf("%-*s\r", FILE_NAME_DISP_WIDTH, main_file_name);
        }

        printf("\x1b[24C");
        if (fin_state == -1) {
            printf("[XXXXXXXXXXXXXXXX(XXXXXXXX)/%016llu]\n", file_size);
        } else if (fin_state == 1) {
            printf("[OOOOOOOOOOOOOOOO(OOOOOOOO)/%016llu]\n", file_size);
        } else {
            printf("[%016llu(%08u)/%016llu]\n", progress, bufdsz, file_size);
        }
        if (fin_state == 0) {
            printf("rtt=%08u, ", rtt);
            printf("rto=%08u, ", rto);
            printf("dac=%04u, ", dac);
            printf("cwnd=%04u, ", cwnd);
            printf("stoc=%08u, ", send_timeout_count);
            printf("frxc=%08u\r\x1b[A", fast_retransmit_count);
        } else {
            printf("\x1b[A");
        }

        if (0 < index) {
            printf("\x1b[%uA", index * 2);
        }

        fflush(stdout);

        pthread_mutex_unlock(disp_mutex);
    }

#   define disp_fin(count) do{unsigned u; unsigned v = (count); for (u = 0 ; u < v ; u++) {putchar('\n'); putchar('\n');}}while (0)

#   define disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count) \
        disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count)
#else
    static void disp(unsigned index, char const *main_file_name, uint64_t file_size, uint64_t progress, int fin_state)
    {
        unsigned u;
        unsigned ratio;
        static pthread_mutex_t disp_mutex[1] = {PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP};
#       define FILE_NAME_DISP_WIDTH (24)
#       define disp_num(n) do               \
        {                                   \
            unsigned num;                   \
            char *au;                       \
            if (10000000000000 < (n)) {     \
                num = (n) / 1000000000000;  \
                au = "T";                   \
            } else if (10000000000 < (n)) { \
                num = (n) / 1000000000;     \
                au = "G";                   \
            } else if (10000000 < (n)) {    \
                num = (n) / 1000000;        \
                au = "M";                   \
            } else if (10000 < (n)) {       \
                num = (n) / 1000;           \
                au = "K";                   \
            } else {                        \
                num = (n);                  \
                au = "";                    \
            }                               \
            printf("%u%sB", num, au);       \
        } while(0)

        if (file_size == 0) {
            ratio = 0;
        } else {
            ratio = progress * 32 / file_size;
        }

        pthread_mutex_lock(disp_mutex);

        for (u = 0 ; u < index ; u++) {
            putchar('\n');
        }

        if (main_file_name == NULL) {
            printf("------------------------\r");
        }
        else if (FILE_NAME_DISP_WIDTH < strlen(main_file_name)) {
            printf("%.*s...\r", FILE_NAME_DISP_WIDTH - 3, main_file_name);
        } else {
            printf("%-*s\r", FILE_NAME_DISP_WIDTH, main_file_name);
        }

        printf("\x1b[24C");
        if (file_size == 0) {
            if (fin_state == -1) {
                printf("[XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX] -/-");
            } else if (fin_state == 1) {
                printf("[OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO] -/-");
            } else {
                printf("[--------------------------------] -/-");
            }
        } else {
            if (fin_state == -1) {
                printf("[XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX] -/");
                disp_num(file_size);
            } else if (fin_state == 1) {
                printf("[OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO] ");
                disp_num(progress);
                putchar('/');
                disp_num(file_size);
            } else {
                printf("[%.*s%.*s] ",
                    ratio, "================================",
                    32 - ratio, "--------------------------------");
                disp_num(progress);
                putchar('/');
                disp_num(file_size);
            }
        }
        printf("\x1b[K\r");

        if (0 < index) {
            printf("\x1b[%uA", index);
        }

        fflush(stdout);

        pthread_mutex_unlock(disp_mutex);
    }

#   define disp_fin(count) do{unsigned u; unsigned v = (count); for (u = 0 ; u < v ; u++) {putchar('\n');}}while (0)

#   define disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count) \
        disp(index, main_file_name, file_size, progress, fin_state)
#endif

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
    int ret;
    unsigned char *message = ((struct file_tx_attr *)param)->message;
    unsigned *message_size = &(((struct file_tx_attr *)param)->message_size);
#   define get_random(buf, buf_sz) do \
    {                                 \
        unsigned char *b = (buf);     \
        unsigned u = (buf_sz);        \
        ssize_t s;                    \
        while (0 < u) {               \
            s = getrandom(b, u, 0);   \
            if (0 < s) {              \
                b += s;               \
                u -= s;               \
            }                         \
        }                             \
    } while(0)
    if (arg_public_key == NULL) {
        *message_size = CONNECT_MESSAGE_SIZE; /* dummy size */
        ret = callback(sid, NULL, 0, 0);
    } else {
        *message_size = CONNECT_MESSAGE_SIZE;
        get_random(message, *message_size);
        ret = callback(sid, message, *message_size, 0);
    }
    return ret;
}

static int recv_connect_payload(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param)
{
    int ret;
    unsigned char *message = ((struct file_tx_attr *)param)->message;
    unsigned *message_size = &(((struct file_tx_attr *)param)->message_size);
    unsigned char sign_message[CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE];
    unsigned sign_message_size = CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE;
    ret = callback(sid, sign_message, &sign_message_size);
    if ((arg_public_key != NULL) &&
            ((ret != 0) ||
                (sign_message_size != CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE) ||
                (memcmp(message, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, CONNECT_MESSAGE_SIZE) != 0) ||
                (cipher_ed25519_auth(sign_message, arg_public_key, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, sign_message_size - CIPHER_ED25519_SIGNATURE_SIZE) != 0))) {
        *message_size = 0;
    }
    return ret;
}

static int conn(struct file_tx_attr *attr,
    struct sockaddr_storage const *address,
    unsigned timeout, int *is_running)
{
    int i;
    unsigned t;
    void **sid = &(attr->sid);
    void *new_sid;
    void *old_sid;
    unsigned const start_time = espress_stream_time();
    struct espress_stream_session_ex_param ex_param[1] = {{
        send_connect_payload, NULL,
        recv_connect_payload, NULL,
        NULL, NULL,
        NULL, NULL
    }};
    unsigned *message_size = &(attr->message_size);

    i = espress_stream_connect(*sid, address);
    if (i == 0) {
        for (i = -1, t = 0 ; *is_running && (i == -1) ; ) {
            t = timeout - t;
            i = espress_stream_connect_result(*sid, t);
            if (0 < i) {
                ex_param->send_connect_payload_param = attr;
                ex_param->recv_connect_payload_param = attr;
                i = espress_stream_begin_session(&new_sid, ifd, 0, ex_param);
                if (i == 0) {
                    old_sid = *sid;
                    *sid = new_sid;
                    espress_stream_end_session(old_sid);
                    i = espress_stream_connect(*sid, address);
                    if (i == 0) {
                        i = -1;
                    }
                }
            } else if ((i == 0) && (*message_size == 0)) {
                espress_stream_disconnect(*sid);
                espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
                i = 1;
            }

            if (timeout <= (t = (espress_stream_time() - start_time) & 0xffffffff)) {
                break;
            }
        }
    }

    return i;
}

static int send_recv(struct file_tx_attr *attr,
    void const *send_data, unsigned *send_data_size,
    void *recv_data, unsigned *recv_data_size,
    unsigned timeout, int *is_running)
{
    int i;
    int j;
    unsigned t;
    void *sid = attr->sid;
    unsigned const start_time = espress_stream_time();

    for (i = -1, j = -1, t = timeout ; *is_running && (i == -1) && (j == -1) ; ) {
        t = timeout - t;
        espress_stream_wait_any(sid, ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV, t);
        i = espress_stream_send(sid, send_data, send_data_size, 0);
        j = espress_stream_recv(sid, recv_data, recv_data_size, 0);

        if (timeout <= (t = (espress_stream_time() - start_time) & 0xffffffff)) {
            break;
        }
    }

    i = (i < 0) ? -1 : ((0 < i) ? 1 : 0);
    j = (j < 0) ? -1 : ((0 < j) ? 1 : 0);
    return (i & 0x03) | ((j & 0x03) << 2);
}

static int flush_recv(struct file_tx_attr *attr,
    void *recv_data, unsigned *recv_data_size,
    unsigned timeout, int *is_running)
{
    int i;
    int j;
    unsigned t;
    void *sid = attr->sid;
    unsigned const start_time = espress_stream_time();

    for (i = -1, j = -1, t = timeout ; *is_running && (i == -1) && (j == -1) ; ) {
        t = timeout - t;
        espress_stream_wait_any(sid, ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV, t);
        i = espress_stream_flush(sid, 0);
        j = espress_stream_recv(sid, recv_data, recv_data_size, 0);

        if (timeout <= (t = (espress_stream_time() - start_time) & 0xffffffff)) {
            break;
        }
    }

    i = (i < 0) ? -1 : ((0 < i) ? 1 : 0);
    j = (j < 0) ? -1 : ((0 < j) ? 1 : 0);
    return (i & 0x03) | ((j & 0x03) << 2);
}

static void *file_tx(void *param)
{
    int i;
    int j;
    size_t s;
    unsigned t;
    unsigned u;
    unsigned v;
    unsigned w;
    unsigned char b;
    void *ret = param;
    struct sockaddr_storage const *address = ((struct file_tx_attr *)param)->address;
    unsigned index = ((struct file_tx_attr *)param)->index;
    int *is_running = &(((struct file_tx_attr *)param)->is_running);
    char const *file_name = ((struct file_tx_attr *)param)->file_name;
    char const *main_file_name;
    FILE *file;
    uint64_t file_size = 0;
    void **sid = &(((struct file_tx_attr *)param)->sid);
    unsigned char buf[BUFFER_SIZE];
    uint64_t progress;
    unsigned start_time;

    main_file_name = strrchr(file_name, '/');
    if (main_file_name == NULL) {
        main_file_name = file_name;
    } else {
        if (*(++main_file_name) == '\0') {
            main_file_name = NULL;
            ret = NULL;
            goto FILE_NAME_INVALID_EXIT;
        }
    }

    file = fopen(file_name, "rb");
    if (file == NULL) {
        ret = NULL;
        goto FILE_OPEN_FAILURE_EXIT;
    }

    disp(index, main_file_name, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    i = conn((struct file_tx_attr *)param, address, *is_running ? TIMEOUT : 0, is_running);
    if (i != 0) {
        ret = NULL;
        goto CONNECT_FAILURE_EXIT;
    }

    strcpy((char *)buf, arg_user_id);
    strcat((char *)buf, ":");
    strcat((char *)buf, arg_user_password);
    for (u = 0, s = strlen((char *)buf) + 1 ; u < s ; u += v) {
        v = s - u;
        w = 1;
        i = send_recv((struct file_tx_attr *)param, buf + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
        j = (i >> 2) & 0x03; if (1 < j) j = -1;
        i = i & 0x03; if (1 < i) i = -1;
        if (i != 0) {
            i = 1;
            break;
        }
        if ((j == 0) && (w == 0)) {
            i = 1;
            break;
        }
        i = 0;
    }
    if (i != 0) {
        ret = NULL;
        goto USER_ID_PASS_SEND_FAILURE_EXIT;
    }
    i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
    if (i != 0) {
        ret = NULL;
        goto USER_ID_PASS_SEND_FAILURE_EXIT;
    }

    for (u = 0, s = strlen(main_file_name) + 1 ; u < s ; u += v) {
        v = s - u;
        w = 1;
        i = send_recv((struct file_tx_attr *)param, main_file_name + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
        j = (i >> 2) & 0x03; if (1 < j) j = -1;
        i = i & 0x03; if (1 < i) i = -1;
        if (i != 0) {
            i = 1;
            break;
        }
        if ((j == 0) && (w == 0)) {
            i = 1;
            break;
        }
        i = 0;
    }
    if (i != 0) {
        ret = NULL;
        goto NAME_SEND_FAILURE_EXIT;
    }
    i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
    if (i != 0) {
        ret = NULL;
        goto NAME_SEND_FAILURE_EXIT;
    }

    for (progress = 0 ; *is_running ; ) {
        s = fread(buf, 1, sizeof(buf), file);
        for (u = 0 ; u < s ; u += v) {
            disp(index, main_file_name, file_size, (progress + u) - espress_stream_buffered_send_size(*sid, NULL), i ? -1 : 0,
                espress_stream_buffered_send_size(*sid, NULL),
                espress_stream_rtt(*sid),
                espress_stream_rto(*sid),
                espress_stream_dac(*sid),
                espress_stream_cwnd(*sid),
                espress_stream_send_timeout_count(*sid),
                espress_stream_fast_retransmit_count(*sid));
            v = s - u;
            w = 1;
            i = send_recv((struct file_tx_attr *)param, buf + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
            j = (i >> 2) & 0x03; if (1 < j) j = -1;
            i = i & 0x03; if (1 < i) i = -1;
            if (i != 0) {
                i = 1;
                break;
            }
            if ((j == 0) && (w == 0)) {
                i = 1;
                break;
            }
        }
        disp(index, main_file_name, file_size, (progress += u) - espress_stream_buffered_send_size(*sid, NULL), i ? -1 : 0,
            espress_stream_buffered_send_size(*sid, NULL),
            espress_stream_rtt(*sid),
            espress_stream_rto(*sid),
            espress_stream_dac(*sid),
            espress_stream_cwnd(*sid),
            espress_stream_send_timeout_count(*sid),
            espress_stream_fast_retransmit_count(*sid));
        if (i != 0) {
            break;
        }
        if (s < sizeof(buf)) {
            break;
        }
    }
    if (!*is_running || (i != 0)) {
        ret = NULL;
        goto DATA_SEND_BREAK_EXIT;
    }

    for (u = espress_stream_buffered_send_size(*sid, NULL), start_time = espress_stream_time(), t = 0 ;
        *is_running ;
        u = v) {
        disp(index, main_file_name, file_size, progress - espress_stream_buffered_send_size(*sid, NULL), 0,
            espress_stream_buffered_send_size(*sid, NULL),
            espress_stream_rtt(*sid),
            espress_stream_rto(*sid),
            espress_stream_dac(*sid),
            espress_stream_cwnd(*sid),
            espress_stream_send_timeout_count(*sid),
            espress_stream_fast_retransmit_count(*sid));
        w = 1;
        i = flush_recv((struct file_tx_attr *)param, &b, &w, 0, is_running);
        j = (i >> 2) & 0x03; if (1 < j) j = -1;
        i = i & 0x03; if (1 < i) i = -1;
        if ((0 <= i) || ((j == 0) && (w == 0))) {
            break;
        }
        v = espress_stream_buffered_send_size(*sid, NULL);
        if (u != v) {
            start_time = espress_stream_time();
        }

        if ((t = (espress_stream_time() - start_time) & 0xffffffff) < TIMEOUT) {
            t = TIMEOUT - t;
            espress_stream_wait_any(*sid, ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV, *is_running ? t : 0);
        } else {
            break;
        }
    }
    if (!*is_running || (i != 0) || ((j == 0) && (w == 0))) {
        ret = NULL;
        goto DATA_SEND_TERMINATE_FAILURE_EXIT;
    }

DATA_SEND_TERMINATE_FAILURE_EXIT:
DATA_SEND_BREAK_EXIT:
NAME_SEND_FAILURE_EXIT:
USER_ID_PASS_SEND_FAILURE_EXIT:
    espress_stream_disconnect(*sid);
    espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
    if (ret != NULL) {
        for (u = 0 ; u < sizeof(uint64_t) ; u += v) {
            v = sizeof(uint64_t) - u;
            i = espress_stream_recv(*sid, buf + u, &v, *is_running ? TIMEOUT : 0);
            if (i != 0) {
                break;
            }
            if (v == 0) {
                i = 1;
                break;
            }
        }
        if (!*is_running || (i != 0) || (file_size != be64toh(*((uint64_t *)buf)))) {
            ret = NULL;
        } else {
            w = 1;
            espress_stream_recv(*sid, &b, &w, *is_running ? TIMEOUT : 0);
        }
    }
CONNECT_FAILURE_EXIT:
    fclose(file);
FILE_OPEN_FAILURE_EXIT:
FILE_NAME_INVALID_EXIT:
    if (ret == NULL) {
        disp(index, main_file_name, file_size, 0, -1, 0, 0, 0, 0, 0, 0, 0);
    } else {
        disp(index, main_file_name, file_size, file_size, 1, 0, 0, 0, 0, 0, 0, 0);
    }

    *is_running = 0;
    return ret;
}

static void sigh(int signum)
{
    unsigned u;
    for (u = 0 ; u < arg_num_file ; u++) {
        if (file_tx_attr[u].is_attached) {
            file_tx_attr[u].is_running = 0;
        }
    }
}

int main(int argc, char const **argv)
{
    int ret = 0;
    int i;
    unsigned u;
    void *p;
    int is_stream_ng = 1;
    struct sockaddr_storage address[1] = {0};
    struct sockaddr_storage src_address[1] = {0};
    sighandler_t org_sigh;
    struct espress_stream_session_ex_param ex_param[1] = {{
        send_connect_payload, NULL,
        recv_connect_payload, NULL,
        NULL, NULL,
        NULL, NULL
    }};
    char user_id[USER_ID_SIZE];
    char user_password[USER_PASSWORD_SIZE];

    ifd = NULL;

    i = scan_arg(argc, argv);
    if (i) {
        ret = 1;
        //fprintf(stderr, "error: scan argument failure.\n");
        goto EXIT;
    }

    printf("destination address: %s\n", arg_address);
    printf("destination port: %s\n", arg_port);
    printf("destination public key: ");
    if (arg_public_key == NULL) {
        printf("(none)");
    } else {
        for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
            printf("%02x", arg_public_key[i]);
        }
    }
    printf("\n");
    printf("number of sending files: %u\n", arg_num_file);
    printf("user id: ");
    if (arg_user_id == NULL) {
        if ((fgets(user_id, USER_ID_SIZE, stdin) == NULL) ||
            (user_id[0] == '\n') ||
            (user_id[0] == '\0')) {
            user_id[0] = '\0';
        } else if (user_id[strlen(user_id) - 1] == '\n') {
            user_id[strlen(user_id) - 1] = '\0';
        }
    } else {
        strncpy(user_id, arg_user_id, USER_ID_SIZE);
        user_id[USER_ID_SIZE - 1] = '\0';
        printf("%s\n", user_id);
    }
    arg_user_id = user_id;
    printf("user password: ");
    if (arg_user_password == NULL) {
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-result"
        system("stty -echo");
#       pragma GCC diagnostic pop
        if ((fgets(user_password, USER_PASSWORD_SIZE, stdin) == NULL) ||
            (user_password[0] == '\n') ||
            (user_password[0] == '\0')) {
            user_password[0] = '\0';
        } else if (user_password[strlen(user_password) - 1] == '\n') {
            user_password[strlen(user_password) - 1] = '\0';
        }
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wunused-result"
        system("stty echo");
#       pragma GCC diagnostic pop
        printf("\n");
    } else {
        strncpy(user_password, arg_user_password, USER_PASSWORD_SIZE);
        user_password[USER_PASSWORD_SIZE - 1] = '\0';
        printf("\n");
    }
    arg_user_password = user_password;

    i = inet_naton(arg_address, arg_port, (struct sockaddr *)address);
    if (i) {
        ret = 1;
        fprintf(stderr, "error: invalid address/port\n");
        goto EXIT;
    }

    file_tx_attr = (struct file_tx_attr *)calloc(arg_num_file, sizeof(struct file_tx_attr));
    if (file_tx_attr == NULL) {
        ret = 1;
        fprintf(stderr, "error: heap allocate failure.\n");
        goto EXIT;
    }

    is_stream_ng = (espress_stream_begin() != 0);
    if (is_stream_ng) {
        ret = 1;
        fprintf(stderr, "error: can't begin stream.\n");
        goto EXIT;
    }

    ((struct sockaddr_in *)src_address)->sin_family = AF_INET;
    ((struct sockaddr_in *)src_address)->sin_port = htons(0);
    ((struct sockaddr_in *)src_address)->sin_addr.s_addr = htonl(INADDR_ANY);
    i = espress_stream_begin_interface(&ifd, src_address);
    if (i != 0) {
        ifd = NULL;
        ret = 1;
        fprintf(stderr, "error: can't begin interface.\n");
        goto EXIT;
    }

    for (u = 0 ; u < arg_num_file ; u++) {
        file_tx_attr[u].index = u;
        file_tx_attr[u].is_attached = 1;
        file_tx_attr[u].is_running = 1;
        file_tx_attr[u].address = address;
        file_tx_attr[u].file_name = arg_file_name[u];
        file_tx_attr[u].ret = 0;
        ex_param->send_connect_payload_param = file_tx_attr + u;
        ex_param->recv_connect_payload_param = file_tx_attr + u;
        i = espress_stream_begin_session(&(file_tx_attr[u].sid), ifd, 0, ex_param);
        if ((i != 0) || pthread_create(&(file_tx_attr[u].thread), NULL, file_tx, file_tx_attr + u)) {
            file_tx_attr[u].is_attached = 0;
            file_tx_attr[u].is_running = 0;
        }
    }

    org_sigh = signal(SIGINT, sigh);

    for ( ; ; ) {
        sleep(1);
        for (u = 0 ; u < arg_num_file ; u++) {
            if (file_tx_attr[u].is_attached && file_tx_attr[u].is_running) {
                break;
            }
        }
        if (u == arg_num_file) {
            for (u = 0 ; u < arg_num_file ; u++) {
                if (file_tx_attr[u].is_attached) {
                    espress_stream_wait_break(file_tx_attr[u].sid);
                }
            }
            break;
        }
    }

    signal(SIGINT, org_sigh);

    for (u = 0 ; u < arg_num_file ; u++) {
        if (file_tx_attr[u].is_attached) {
            i = pthread_join(file_tx_attr[u].thread, &p);
            if ((i != 0) || (p == NULL)) {
                file_tx_attr[u].ret = 1;
            }
            file_tx_attr[u].is_attached = 0;
            espress_stream_end_session(file_tx_attr[u].sid);
        } else {
            file_tx_attr[u].ret = 1;
        }
    }

    disp_fin(arg_num_file);

    for (u = 0 ; u < arg_num_file ; u++) {
        if (file_tx_attr[u].ret) {
            ret = 1;
            fprintf(stderr, "error: file \"%s\" transmit failure.\n", file_tx_attr[u].file_name);
        }
    }


EXIT:

    if (ifd != NULL) espress_stream_end_interface(ifd);
    if (!is_stream_ng) espress_stream_end();
    if (file_tx_attr != NULL) free(file_tx_attr);

    return ret;
}
```

</details>

<details><summary><code>arch/linux/app/es_frx/src/main.c</code></summary>

```
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <endian.h>
#include <signal.h>
#include <unistd.h>

#include "espress_stream/api.h"
#include "cipher_api.h"
#include "cert.h"

static unsigned char const secret_key[] = SECRET_KEY;
static unsigned char const public_key[] = PUBLIC_KEY;
static char const *user_id_pass[] = USER_ID_PASS;

#define IS_DEBUGGING (0)
#define NUM_STANDBY (16)
#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (4 *1024)
#define CONNECT_MESSAGE_SIZE (64)

#if IS_DEBUGGING
#   define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#   define DEBUG_PRINT(...) ((void)0)
#endif

static void *ifd;
static const char *arg_address;
static const char *arg_port;
static int arg_is_overwrite;
static const char *arg_dir_name;

static int inet_naton(char const *name, char const *port, struct sockaddr *address)
{
    int ret;
    struct addrinfo hint[1] = {0};
    struct addrinfo *addrinfo;
    hint->ai_family = AF_INET;
    hint->ai_socktype = SOCK_DGRAM;
    if (name == NULL) {
        hint->ai_flags = AI_PASSIVE;
    }
    ret = getaddrinfo(name, port, hint, &addrinfo);
    if (ret == 0) {
        memcpy(address, addrinfo->ai_addr, addrinfo->ai_addrlen);
        freeaddrinfo(addrinfo);
    }
    return ret;
}

static int scan_arg(int argc, char const **argv)
{
    int i;
    char const *p;
    int ret;
    char c = '\0';

    arg_address = NULL;
    arg_port = "12345";
    arg_is_overwrite = 0;
    arg_dir_name = NULL;

    for (i = 1 ; i < argc ; i++) {
                if (argv[i][0] == '-') {
            c = argv[i][1];
            switch (c) {
            case 'a':
            case 'p':
                break;
            case 'o':
                arg_is_overwrite = 1;
                c = '\0';
                break;
            default:
                c = '\0';
                break;
            }
            continue;
        }
        switch (c) {
        case 'a':
            arg_address = argv[i];
            c = '\0';
            break;
        case 'p':
            arg_port = argv[i];
            c = '\0';
            break;
        default:
            if (arg_dir_name == NULL) {
                arg_dir_name = argv[i];
                ret = 0;
            } else {
                ret = 1;
            }
            break;
        }
        if (ret) {
            break;
        }
    }

    ret = ret || (arg_dir_name == NULL);

    if (ret) {
        p = strrchr(argv[0], '/');
        if (p == NULL) {
            p = argv[0];
        } else {
            p++;
        }
        printf("%s "
                "[-a <bind address>] "
                "[-p <bind port>] "
                "[-o (overwrite)] "
                "<dir name>\n",
            p);
        printf("%s (C) 2023 akinoyonika\n", p);
        printf("%s\n", espress_stream_copyright());
    }

    return ret || (arg_dir_name == NULL);
}

struct file_rx_attr {
    int is_attached;
    pthread_t thread;
    int is_running;
    void *sid;
};

static void *file_rx(void *param)
{
    void *ret = param;
    int i;
    unsigned u;
    unsigned v;
    size_t s;
    int *is_running = &(((struct file_rx_attr *)param)->is_running);
    void **sid = &(((struct file_rx_attr *)param)->sid);
    unsigned char buf[BUFFER_SIZE];
    char file_name[BUFFER_SIZE];
    FILE *file;
    uint64_t file_size;

    for (u = 0 ; u < sizeof(buf) ; u++) {
        v = 1;
        i = espress_stream_recv(*sid, buf + u, &v, *is_running ? TIMEOUT : 0);
        if (i != 0) {
            break;
        }
        if (v == 0) {
            i = 1;
            break;
        }
        if (buf[u] == '\0') {
            break;
        }
    }
    if ((i != 0) || (u == sizeof(buf))) {
        ret = NULL;
        goto USER_ID_PASS_SCAN_FAILURE_EXIT;
    }
    for (i = 0 ; i < (sizeof(user_id_pass) / sizeof(user_id_pass[0])) ; i++) {
        if (strcmp((char *)buf, user_id_pass[i]) == 0) {
            break;
        }
    }
    if (i == (sizeof(user_id_pass) / sizeof(user_id_pass[0]))) {
        ret = NULL;
        goto USER_ID_PASS_SCAN_FAILURE_EXIT;
    }

    i = (arg_dir_name[strlen(arg_dir_name) - 1] == '/') ? 0 : 1;
    if (sizeof(file_name) <= (strlen(arg_dir_name) + i)) {
        ret = NULL;
        goto DIR_NAME_TOO_LONG_EXIT;
    }
    sprintf(file_name, "%s%s", arg_dir_name, i ? "/" : "");
    for (u = strlen(file_name) ; u < sizeof(file_name) ; u++) {
        v = 1;
        i = espress_stream_recv(*sid, file_name + u, &v, *is_running ? TIMEOUT : 0);
        if (i != 0) {
            break;
        }
        if (v == 0) {
            i = 1;
            break;
        }
        if (file_name[u] == '\0') {
            break;
        }
    }
    if ((i != 0) || (u == sizeof(file_name))) {
        ret = NULL;
        goto FILE_NAME_SCAN_FAILURE_EXIT;
    }

    if (!arg_is_overwrite) {
        file = fopen(file_name, "rb");
        if (file != NULL) {
            fclose(file);
            ret = NULL;
            goto FILE_OPEN_FAILURE_EXIT;
        }
    }

    file = fopen(file_name, "wb");
    if (file == NULL) {
        ret = NULL;
        goto FILE_OPEN_FAILURE_EXIT;
    }

    DEBUG_PRINT("download start: \"%s\"\n", file_name);

    while (*is_running) {
        u = sizeof(buf);
        i = espress_stream_recv(*sid, buf, &u, *is_running ? TIMEOUT : 0);
        if (i != 0) {
            break;
        }
        if (u == 0) {
            break;
        }
        s = fwrite(buf, 1, u, file);
        if (s < u) {
            i = 1;
            break;
        }
    }
    if (!*is_running || (i != 0)) {
        ret = NULL;
        DEBUG_PRINT("download break: \"%s\"\n", file_name);
        goto DATA_RECV_BREAK_EXIT;
    }

    DEBUG_PRINT("download finish: \"%s\"\n", file_name);

DATA_RECV_BREAK_EXIT:
    file_size = ftell(file);
    fclose(file);
    if (ret == NULL) {
        unlink(file_name);
    } else {
        *((uint64_t *)buf) = htobe64(file_size);
        for (u = 0 ; u < sizeof(uint64_t) ; u += v) {
            v = sizeof(uint64_t) - u;
            i = espress_stream_send(*sid, buf + u, &v, *is_running ? TIMEOUT : 0);
            if (i != 0) {
                break;
            }
        }
        if (i != 0) {
            ret = NULL;
            DEBUG_PRINT("send response failure: \"%s\"\n", file_name);
        } else {
            i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
            if (i != 0) {
                ret = NULL;
                DEBUG_PRINT("send response failure: \"%s\"\n", file_name);
            } else {
                DEBUG_PRINT("send response: \"%s\"\n", file_name);
            }
        }
    }
FILE_OPEN_FAILURE_EXIT:
FILE_NAME_SCAN_FAILURE_EXIT:
USER_ID_PASS_SCAN_FAILURE_EXIT:
DIR_NAME_TOO_LONG_EXIT:
    espress_stream_disconnect(*sid);
    espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);

    *is_running = 0;
    return ret;
}

struct standby_attr {
    int is_attached;
    pthread_t thread;
    int is_running;
    unsigned char message[CONNECT_MESSAGE_SIZE];
    unsigned message_size;
};
static struct standby_attr standby_attr[NUM_STANDBY];

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
    int ret;
    unsigned char *message = ((struct standby_attr *)param)->message;
    unsigned message_size = ((struct standby_attr *)param)->message_size;
    unsigned char sign_message[CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE];
    if (message_size == CONNECT_MESSAGE_SIZE) {
        cipher_ed25519_sign(sign_message, secret_key, message, message_size);
        memcpy(sign_message + CIPHER_ED25519_SIGNATURE_SIZE, message, message_size);
        ret = callback(sid, sign_message, CIPHER_ED25519_SIGNATURE_SIZE + message_size, 0);
    } else {
        ret = callback(sid, NULL, 0, 0);
    }
    return ret;
}

static int recv_connect_payload(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param)
{
    unsigned char *message = ((struct standby_attr *)param)->message;
    unsigned *message_size = &(((struct standby_attr *)param)->message_size);
    *message_size = CONNECT_MESSAGE_SIZE;
    return callback(sid, message, message_size);
}

static void *standby(void *param)
{
    void *ret = param;
    int i;
    void *p;
    void *q;
    unsigned t;
    unsigned u;
    int *is_running = &(((struct standby_attr *)param)->is_running);
    unsigned num_thread = 0;
    struct file_rx_attr **attr = NULL;
    struct espress_stream_session_ex_param ex_param[1] = {{
        send_connect_payload, NULL,
        recv_connect_payload, NULL,
        NULL, NULL,
        NULL, NULL
    }};

    while (*is_running) {
        for (t = 0, u = num_thread ; t < num_thread ; t++) {
            if (!attr[t]->is_attached) {
                u = t;
                if (attr[t]->sid != NULL) {
                    break;
                }
            }
        }
        if (t == num_thread) {
            t = u;
        }
        if (t == num_thread) {
            num_thread++;
            p = realloc(attr, sizeof(struct file_rx_attr *) * num_thread);
            q = malloc(sizeof(struct file_rx_attr));
            if ((p == NULL) || (q == NULL)) {
                if (p != NULL) free(p);
                if (q != NULL) free(q);
                num_thread--;
            } else {
                attr = (struct file_rx_attr **)p;
                attr[t] = (struct file_rx_attr *)q;
                attr[t]->is_attached = 0;
                attr[t]->is_running = 0;
                attr[t]->sid = NULL;
            }
        }

        if (t < num_thread) {
            if (attr[t]->sid == NULL) {
                ex_param->send_connect_payload_param = param;
                ex_param->recv_connect_payload_param = param;
                if (espress_stream_begin_session(&(attr[t]->sid), ifd, 1, ex_param) != 0) {
                    attr[t]->sid = NULL;
                }
            }
            if (attr[t]->sid != NULL) {
                i = espress_stream_connect_detect(attr[t]->sid, 1 * 1000 * 1000);
                if (i == 0) {
                    attr[t]->is_attached = 1;
                    attr[t]->is_running = 1;
                    if (pthread_create(&(attr[t]->thread), NULL, file_rx, attr[t])) {
                        espress_stream_disconnect(attr[t]->sid);
                        espress_stream_disconnect_result(attr[t]->sid, *is_running ? TIMEOUT : 0);
                        espress_stream_end_session(attr[t]->sid);
                        attr[t]->sid = NULL;
                        attr[t]->is_attached = 0;
                        attr[t]->is_running = 0;
                    }
                } else if (0 < i) {
                    espress_stream_end_session(attr[t]->sid);
                    attr[t]->sid = NULL;
                }
            }
        }

        for (t = 0 ; t < num_thread ; t++) {
            if (attr[t]->is_attached && !attr[t]->is_running) {
                pthread_join(attr[t]->thread, NULL);
                attr[t]->is_attached = 0;
                espress_stream_end_session(attr[t]->sid);
                attr[t]->sid = NULL;
            }
        }
    }

    for (t = 0 ; t < num_thread ; t++) {
        if (attr[t]->is_attached) {
            attr[t]->is_running = 0;
        }
    }
    for (t = 0 ; t < num_thread ; t++) {
        if (attr[t]->is_attached) {
            espress_stream_wait_break(attr[t]->sid);
            pthread_join(attr[t]->thread, NULL);
            attr[t]->is_attached = 0;
            espress_stream_end_session(attr[t]->sid);
            attr[t]->sid = NULL;
        }
    }

    for (t = 0 ; t < num_thread ; t++) {
        free(attr[t]);
    }
    free(attr);

    *is_running = 0;
    return ret;
}

static void sigh(int signum)
{
    unsigned u;
    for (u = 0 ; u < NUM_STANDBY ; u++) {
        standby_attr[u].is_running = 0;
    }
}

int main(int argc, char const **argv)
{
    int ret = 0;
    int i;
    unsigned u;
    int is_stream_ng = 1;
    struct sockaddr_storage address[1];
    sighandler_t org_sigh;

    i = scan_arg(argc, argv);
    if (i) {
        ret = 1;
        //fprintf(stderr, "error: scan argument failure.\n");
        goto EXIT;
    }

    printf("public key: ");
    for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
        printf("%02x", public_key[i]);
    }
    printf("\n");
    printf("bind address: %s%s%s\n",
        (arg_address == NULL) ? "(" : "",
        (arg_address == NULL) ? "any" : arg_address,
        (arg_address == NULL) ? ")" : "");
    printf("bind port: %s\n", arg_port);
    printf("receiving file overwrite: %s\n", arg_is_overwrite ? "available" : "unavailable");
    printf("receiving file store dir: \"%s\"\n", arg_dir_name);

    is_stream_ng = (espress_stream_begin() != 0);
    if (is_stream_ng) {
        ret = 1;
        fprintf(stderr, "error: can't begin stream.\n");
        goto EXIT;
    }
    i = inet_naton(arg_address, arg_port, (struct sockaddr *)address);
    if (i) {
        ret = 1;
        fprintf(stderr, "error: invalid address/port\n");
        goto EXIT;
    }

    i = espress_stream_begin_interface(&ifd, address);
    if (i != 0) {
        ifd = NULL;
        ret = 1;
        fprintf(stderr, "error: can't begin interface.\n");
        goto EXIT;
    }

    for (u = 0 ; u < NUM_STANDBY ; u++) {
        standby_attr[u].is_attached = 1;
        standby_attr[u].is_running = 1;
        if (pthread_create(&(standby_attr[u].thread), NULL, standby, standby_attr + u)) {
            standby_attr[u].is_attached = 0;
            standby_attr[u].is_running = 0;
        }
    }

    org_sigh = signal(SIGINT, sigh);

    for (u = 0 ; u < NUM_STANDBY ; u++) {
        if (standby_attr[u].is_attached) {
            pthread_join(standby_attr[u].thread, NULL);
            standby_attr[u].is_attached = 0;
        }
    }

    signal(SIGINT, org_sigh);

EXIT:

    if (ifd != NULL) espress_stream_end_interface(ifd);
    if (!is_stream_ng) espress_stream_end();

    return ret;
}
```

</details>

### サンプルアプリケーション (<tt>es_ftx</tt>, <tt>es_frx</tt>)

クライアントアプリケーション(<tt>es_ftx</tt>)からサーバーアプリケーション(<tt>es_frx</tt>)にファイルを送信します。

まず、<tt>es_frx</tt>をファイルを受信するマシンで立ち上げます。

```
$ es_frx ~/Downloads
public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
bind address: (any)
bind port: 12345
receiving file overwrite: unavailable
receiving file store dir: "/home/foo/Downloads"
```

<tt>~/Downloads</tt>は受信したファイルを格納するディレクトリです。
<tt>public key</tt>はクライアントが行うサーバー認証のための公開鍵です。
受信ファイルと同名のファイルが存在する場合、エラーとなりますが、上書きしても構わない場合は<tt>-o</tt>オプションを指定してください。
また、ポート番号はデフォルトで<tt>12345</tt>になっていますので、変更したい場合は<tt>-p</tt>オプションを使用してください。

```
$ es_frx -p 12345 -o ~/Downloads
public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
bind address: (any)
bind port: 12345
receiving file overwrite: available
receiving file store dir: "~/Downloads"
```

次に、ファイルを送信するマシンで<tt>es_ftx</tt>を立ち上げ、ファイル送信を行います。

```
$ es_ftx -a foo -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9 ~/Documents/boo.doc ~/Music/boo.mp3 ~/Pictures/boo.jpg ~/Videos/boo.mp4
destination address: localhost
destination port: 12345
destination public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
number of sending files: 4
user id: foo
user password: 
    :
    :
```
<tt>-a foo</tt>は受信マシンのドメイン名指定です (<tt>###.###.###.###</tt>形式でIPアドレス指定可)。
<tt>-k 7335...</tt>はサーバーの公開鍵です。サーバー実行時に表示されるものをそのまま指定してください。
<tt>~/Documents/boo.doc ... ~/Videos/boo.mp4</tt>は送信するファイルの指定です (任意の数のファイルを指定可)。
サーバーアプリケーションは指定格納ディレクトリに主ファイル名でファイルを格納しますので、異なるファイルでも同じ主ファイル名を持つファイルは送信できません。
実行するとユーザーIDとパスワードの入力が求められますので、入力してください。

<tt>es_frx</tt>と同様、ポート番号はデフォルトで<tt>12345</tt>になっていますので、変更したい場合は<tt>-p</tt>オプションを使用してください。

```
$ es_ftx -a 192.168.1.1 -p 12345 -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9 ~/Documents/boo.doc ~/Music/boo.mp3 ~/Pictures/boo.jpg ~/Videos/boo.mp4
destination address: 192.168.1.1
destination port: 12345
destination public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
number of sending files: 4
user id: foo
user password: 
    :
    :
```

ファイル送信失敗の検出はクライアント側に表示されますが、サーバー側に受信ファイルが残ってしまうことがあります。
この場合、上書き指定がされていないと同名のファイルを送信できなくなりますのでご注意ください。

<tt>es_ftx</tt>はファイル送信が終了するとソフトウェアも終了します。<tt>es_frx</tt>は<tt>[CTRL] + [C]</tt>で終了します。
どちらも<tt>[CTRL] + [C]</tt>で中断できます。

### サンプルアプリケーションのサーバー認証について

サーバー認証にはEd25519アルゴリズム[RFC 8032]を使用しています。
[OpenSSH](https://www.openssh.com/)の同アルゴリズム使用の認証方法に類似のものとなっています。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2023 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
