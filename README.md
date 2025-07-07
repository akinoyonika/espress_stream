# espress_stream - ストリーム通信用ライブラリー

## 概要

本ソフトウェアはデータグラム通信上でストリーム通信を実現するためのライブラリーです。
認証付き暗号化パケットによりメッセージを秘匿します。

本ソフトウェアのソースコードはC言語によって記述されています。基本部分はアーキテクチャー依存部を分離しており、組み込みシステムを含む各種プラットフォーム、OS上で動作させることができるようデザインされています。
スレッド、ヒープの未使用を選択することも可能です。

本リポジトリではlinuxに対するポーティングが提供されています。
linuxに対するポーティングではデータグラム通信にIPv6およびIPv4のUDPおよびRAWパケットを用いることができます。

主な特徴を以下に示します。

接続
: 1RTT接続

切断
: 1/2RTTハーフクローズ

死活監視
: 無受信期間タイムアウトによる切断検出および無送信データ期間のビーコン送信による活線維持と無応答期間タイムアウトによる通信途絶検出

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

- [espress_stream.20250731.tar.gz](http://akino-yo.blue.coocan.jp/akinoyonika/espress_stream/espress_stream.20250731.tar.gz)

<details><summary>変更履歴</summary>

espress_stream.20250731.tar.gz (1.9-1)
: <ul>
    <li>サンプルアプリケーション<tt>es_gw</tt>を新設。</li>
    <li><tt>espress_stream_recv</tt>が切断検出時にエラーになることがあるバグを修正。</li>
    <li>各サンプルアプリケーションのシグナルによる終了のための処理を効率化。</li>
    <li>アプリケーションがfork-execする場合の対策。</li>
    <li>不正な受信パケットの処理に問題があったので、これを修正。</li>
    <li>セッションを閉じた後に、それに属していたパケットのエンコード、デコード処理が行われてしまうことがあったので、これを修正。</li>
    <li>高速再転送がうまく動作していなかったので、これを修正</li>
  </ul>

espress_stream.20250210.tar.gz (1.8-1)
: <ul>
    <li>linux用<tt>espress_stream_ctr_cipher</tt>に顕在化しないバグあり。修正。</li>
    <li>linux用のいくつかのソースコードの行末がCRLFになっていた。LFに修正。</li>
  </ul>

espress_stream.20250201.tar.gz (1.8-1)
: <ul>
    <li><tt>espress_stream_ctr_cipher</tt>を改良。</li>
    <li>linux用の<tt>espress_stream_ctr_cipher</tt>を作成。OpenMPを使用。</li>
    <li>linux用の<tt>espress_stream_block_cipher</tt>を作成。Kernel Crypt APIを使用。</li>
    <li>linux用の<tt>espress_stream_hash</tt>を作成。Kernel Crypt APIを使用。</li>
  </ul>

espress_stream.20241228.tar.gz (1.7-1)
: <ul>
    <li>各サンプルアプリケーションにおいてSIGINT発生時の終了処理をSIGTERM発生時にも行うようにした。</li>
  </ul>

espress_stream.20241224.tar.gz (1.7-1)
: <ul>
    <li>スレッド情報用リソースが必要以上に多く確保されていたのを修正。</li>
  </ul>

espress_stream.20241214.tar.gz (1.6-1)
: <ul>
    <li>linux実装の<tt>espress_stream_dgram</tt>のRAW対応と修正。</li>
    <li>サンプルアプリケーションのIPv6とRAW対応。</li>
    <li>インターフェイスに属するセッションのリソース列挙に関する部分を修正。</li>
    <li>コードドキュメントの追加。</li>
  </ul>

espress_stream.20240825.tar.gz (1.5-1)
: <ul>
    <li>ドキュメントの修正。</li>
    <li>基本実装及びlinux用実装の<tt>espress_stream_dgram.c</tt>を修正。</li>
  </ul>

espress_stream.20240803.tar.gz (1.4-1)
: <ul>
    <li>通信途絶検出を強化。プロトコル仕様を若干変更。このため、プロトコルバージョンが更新され、以前のバージョンと相互接続できなくなった。</li>
    <li>APIの<tt>espress_stream_send</tt>と<tt>espress_stream_recv</tt>に機能追加。</li>
    <li>サンプルアプリケーション<tt>es_ccat</tt>、<tt>es_scat</tt>のバグ修正、改良。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>の通信途絶検出を改良。</li>
  </ul>


espress_stream.20240710.tar.gz (1.3-0)
: <ul>
    <li>サンプルアプリケーション<tt>es_ccat</tt>、<tt>es_scat</tt>を改良。</li>
  </ul>

espress_stream.20240708.tar.gz (1.3-0)
: <ul>
    <li>サンプルアプリケーション<tt>es_ccat</tt>、<tt>es_scat</tt>を改良。</li>
  </ul>

espress_stream.20240705.tar.gz (1.3-0)
: <ul>
    <li>ドキュメントの修正。</li>
  </ul>

espress_stream.20240704.tar.gz (1.3-0)
: <ul>
    <li>APIの<tt>espress_stream_wait_break</tt>を改良。</li>
    <li>APIに<tt>espress_stream_wait_cancel_break</tt>を新設。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>、<tt>es_frx</tt>を改良。</li>
    <li>サンプルアプリケーション<tt>es_ccat</tt>、<tt>es_scat</tt>を新設。</li>
    <li>サンプルアプリケーションのディレクトリ構成を変更。</li>
    <li>コードドキュメント、<tt>README.md</tt>の修正。</li>
  </ul>

espress_stream.20240117.tar.gz (1.2-0)
: <ul>
    <li>APIの<tt>espress_stream_poll</tt>と<tt>espress_stream_wait_any</tt>を改良。</li>
    <li>前述の改良に合わせてサンプルアプリケーション<tt>es_ftx</tt>を修正。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>に対して<tt>espress_stream_wait_any</tt>の異常動作対応。この関数をコールする場所だけを最適化しないようにした。原因不明。現象確認時のGCCバージョンは11.4.0。</li>
  </ul>

espress_stream.20240115.tar.gz (1.1-0)
: <ul>
    <li>API仕様となる<tt>espress_stream/api.h</tt>のコードドキュメントを加筆修正。</li>
  </ul>

espress_stream.20240112.tar.gz (1.1-0)
: <ul>
    <li>APIの<tt>espress_stream_send</tt>と<tt>espress_stream_recv</tt>を改良。</li>
  </ul>

espress_stream.20240110.tar.gz (1.0-0)
: <ul>
    <li>APIの<tt>espress_stream_wait_for_any</tt>と<tt>espress_stream_poll</tt>のプロトタイプを変更。機能を拡充した。</li>
    <li>APIの仕様変更のため、メジャーバージョン番号を変え、マイナーバージョン番号をリセットした。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>をAPI変更に対応させた。</li>
  </ul>

espress_stream.20231228.tar.gz (0.9-0)
: <ul>
    <li>一部設定値を固定値から初期値付き変数に変更。</li>
    <li>いくつかのモジュールに関して、初期化時のエラーチェックを強化した。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>において、サーバー公開鍵を指定しない場合、サーバー認証を行わないようにした。</li>
    <li>サンプルアプリケーション<tt>es_frx</tt>において、クライアントからのサーバー認証メッセージがない場合、サーバー認証応答もしないようにした。</li>
    <li>サンプルアプリケーションで使用する署名アルゴリズムの実装において固定テーブルの一部が変数になっていたのを固定値に修正。</li>
    <li>多ビット長整数演算部のローカル変数が外部参照可能になっていたのを外部参照不可に修正。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>において、データの送信タイミングが遅れる可能性があったため、これを修正。</li>
    <li>シングルスレッド動作設定時、<tt>espress_stream_poll</tt>内で少なくとも1度は一連のyield処理が呼び出されるようにした。</li>
  </ul>

espress_stream.20231221.tar.gz (0.8-0)
: <ul>
    <li>好ましくない関数名の修正、未削除のコメントアウトコードの削除、テストコードのバグ修正等、動作に影響のないソースコードの修正。</li>
  </ul>

espress_stream.20231219.tar.gz (0.8-0)
: <ul>
    <li>多ビット長整数演算部の改良。</li>
  </ul>

espress_stream.20231214.tar.gz (0.7-0)
: <ul>
    <li>再送タイムアウトにlinuxと同様の最小値を設定した。</li>
    <li>リファクタリング、コメントの修正。</li>
  </ul>

espress_stream.20231209.tar.gz (0.6-0)
: <ul>
    <li>デスティネーションのネットワークアドレスをパケット受信毎に更新するようにした。</li>
    <li>現在のソース及びデスティネーションのネットワークアドレスを取得する機能を追加。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>において、プログレス表示の更新頻度を上げた。</li>
    <li>データグラム通信とのインターフェイスを改良した。</li>
  </ul>

espress_stream.20231202.tar.gz (0.5-0)
: <ul>
    <li>送受信バッファーの使用量を取得する機能を追加。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>において、送信完了タイムアウト検出時に送信バッファーの使用量の変化を考慮するようにし、タイムアウトしにくくした。</li>
    <li>サンプルアプリケーション<tt>es_ftx</tt>において、32ビット環境でも64ビット以上の整数を保証するため、<tt>unsigned long</tt>を<tt>uint64_t</tt>に変更した。</li>
    <li>linux用の各種設定値を調整。</li>
    <li>シングルスレッド動作設定時、アプリケーションのマルチスレッド動作に対して必要な排他制御の一部が欠落していたので、その部分を追加、修正。</li>
  </ul>

espress_stream.20231125.tar.gz (0.4-0)
: <ul>
    <li>サンプルアプリケーション<tt>es_frx</tt>作成時に用いる<tt>mk_cert_h.sh</tt>を改良。</li>
  </ul>

espress_stream.20231124.tar.gz (0.4-0)
: <ul>
    <li>多ビット長整数定義で64ビットの定数定義にあったバグを修正。</li>
    <li>サンプルアプリケーションの認証機能の移植部を内製化。</li>
  </ul>

espress_stream.20231117.tar.gz (0.3-0)
: <ul>
    <li>リトライタイムアウト値の上限をセッションタイムアウト値と同じ値に設定。</li>
    <li>異なるセッションのイベントの発生を同時に待つ機能を追加。</li>
  </ul>

espress_stream.20231107.tar.gz (0.2-0)
: <ul>
    <li>APIレイヤーで、セッションリソースがスタティックでかつシングルスレッドモード時のセッションリソース列挙にバグあり。これを修正。</li>
    <li>linux用の依存定義に対して32ビットコードモデルに適した定義を追加。32ビットコードモデルでのコンパイル時にその定義を使用するようにした。</li>
  </ul>

espress_stream.20231106.tar.gz (0.1-0)
: <ul>
    <li>READMEの僅かな修正のみ。</li>
  </ul>

espress_stream.20231102.tar.gz (0.1-0)
: <ul>
    <li>サンプルアプリケーションに認証機能追加。</li>
    <li>多ビット長整数演算の割り算にあったバグを修正。</li>
  </ul>

espress_stream.20231023.tar.gz (0.0-0)
: <ul>
    <li>初版</li>
</ul>

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

### 手順

```
$ cd espress_stream/arch/linux
$ make clean
$ make
$ sudo make uninstall
$ sudo make install
$ make clean
```

### 注意

<tt>make clean</tt>実行時にシステムの環境に合わせてソースコードの変更を行っています。
このため、インストールするマシン上で<tt>make clean</tt>を実行してください。
もし、ポータブルなライブラリーを作成したい場合は、<tt>make clean</tt>の代わりに<tt>make default-clean</tt>を用いてください。

## 使用方法

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

/** @file espress_stream/api.h
 *  
 * espress_stream APIs
 * 
 * - Function calling procedure.
 * 
 * @verbatim
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
 * @endverbatim
 * 
 * - Async running.
 * 
 *     Different sessions are async runnable.
 *     In single session functions are not able to async run
 *     except for "espress_stream_wait_break" function.
 * 
 * - Return code.
 * 
 *     - == 0:
 *         Complete in success.
 * 
 *     - > 0:
 *         Finished in failure.
 *         In a session when a function return positive value in spite of right procedure,
 *         this session should be destroyed by "espress_stream_end_session" function.
 *         Reprocess from beginning session.
 * 
 *     - == -1:
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
#define ESPRESS_STREAM_EVENT_FLUSH (1 << 5)

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
 *     - == 0: Success.
 *     - > 0 : Failure.
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
 *     - == 0: Success.
 *     - > 0 : Failure.
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
 *     - == 0: Success.
 *     - > 0 : Failure.
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
 *     - == 0: Success.
 *     - > 0 : Failure.
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
 *     - == 0 : Success.
 *     - == -1: Waiting for result yet.
 *     - > 0  : Failure.
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
 *     - == 0 : Success.
 *     - == -1: Waiting for detection yet.
 *     - > 0  : Failure.
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
 *     - == 0: Success.
 *     - > 0 : Failure.
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
 *     - == 0 : Success.
 *     - == -1: Waiting for result yet.
 *     - > 0  : Failure.
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
 *     If NULL is settled, no data is actually sent and the result is simulated.
 * @param[in,out] data_size
 *     Stored address of data size and store address of accepted send request data size.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     - == 0 : Success.
 *     - == -1: Waiting for acceptable state yet.
 *     - > 0  : Failure.
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
 *     - == 0 : Success.
 *     - == -1: Waiting for send complete yet.
 *     - > 0  : Failure.
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
 *     If NULL is settled, no data is actually received and the result is simulated.
 * @param[in,out] data_size
 *     Stored address of data region size and store address of received data size.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     - == 0 : Success.
 *     - == -1: Waiting for incoming data yet.
 *     - > 0  : Failure.
 */
int espress_stream_recv(void *sid, void *data, unsigned *data_size, unsigned timeout);

/** @brief Wait for any event.
 * 
 * Waits until the result of any function with a timeout parameter is available (i.e. return value is greater than or equal to zero).
 * To confirm which function is able to get result, see return value of each function called with zero timeout.
 * Even if this function finish in success, none function's result may be abailable.
 * In this case, repeat call this function.
 * You are able to choose event types which relate the function to wait for result.
 * 
 * | Event types                            | Functions                        |
 * |----------------------------------------|----------------------------------|
 * | ESPRESS_STREAM_EVENT_CONNECT_RESULT    | espress_stream_connect_result    |
 * | ESPRESS_STREAM_EVENT_CONNECT_DETECT    | espress_stream_connect_detect    |
 * | ESPRESS_STREAM_EVENT_DISCONNECT_RESULT | espress_stream_disconnect_result |
 * | ESPRESS_STREAM_EVENT_SEND              | espress_stream_send              |
 * | ESPRESS_STREAM_EVENT_RECV              | espress_stream_recv              |
 * | ESPRESS_STREAM_EVENT_FLUSH             | espress_stream_flush             |
 * 
 * One or more event types are selectable. Set event types concatenated with "|" to the event parameter.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] event
 *     Event types.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     - == 0 : Success.
 *     - == -1: Waiting for event yet.
 *     - > 0  : Failure.
 */
int espress_stream_wait_any(void *sid, unsigned event, unsigned timeout);

/** @brief Break to waiting function
 * 
 * Break to waiting function. Apply this function to the function with timeout parameter.
 * If this function is called while a wait function is running, it return because of timeout that time.
 * If this function is called when no wait function is running, the subsequently executed wait functions return immediately because of  timeout.
 * 
 * @param[in] sid
 *     Session ID.
 */
void espress_stream_wait_break(void *sid);

/** @brief Cancel break to waiting function
 * 
 * This function prevent to immediately return of a subsequently executed waiting function by espress_stream_wait_break function.
 * 
 * @param[in] sid
 *     Session ID.
 */
void espress_stream_wait_cancel_break(void *sid);

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
 *     - >= 0 : Number of detected sids. (Zero indicates timeout.)
 *     - == -1: Failure.
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
単純な例として<tt>arch/linux/app/es_ccat-es_scat/es_ccat/src/main.c</tt> (クライアント)、<tt>arch/linux/app/es_ccat-es_scat/es_scat/src/main.c</tt> (サーバー)を示します。

<details><summary><code>arch/linux/app/es_ccat-es_scat/es_ccat/src/main.c</code></summary>

```
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/random.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include "espress_stream/api.h"
#include "cipher_api.h"

#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (8 * 1024)
#define CONNECT_MESSAGE_SIZE (64)

#define set_eventfd(fd) do{uint64_t u = 1; while (write(fd, &u, sizeof(u)) != sizeof(u));}while (0)
#define wait_eventfd(fd) do{uint64_t u = 1; while (read(fd, &u, sizeof(u)) != sizeof(u));}while (0) 

static void *ifd;
static int arg_protocol;
static char const *arg_address;
static char const *arg_port;
static char const *arg_bind_address;
static char const *arg_bind_port;
static char const *arg_mac_address;
static char const *arg_type;
static char const *arg_ifname;
static unsigned char *arg_public_key;
static unsigned char _arg_public_key[CIPHER_ED25519_PUBLIC_KEY_SIZE];
static int arg_is_close_half;
static int arg_is_silent_enable;

static int inet_naton(int protocol, char const *name, char const *port, struct sockaddr *address)
{
    int ret = 0;
    struct addrinfo hint[1] = {0};
    struct addrinfo *addrinfo = NULL;

    hint->ai_family = protocol;
    hint->ai_socktype = SOCK_DGRAM;
    if (name == NULL) {
        hint->ai_flags = AI_PASSIVE;
    }
    if (port == NULL) {
        port = "0";
    }
    ret = getaddrinfo(name, port, hint, &addrinfo);
    if (ret != 0) {
        addrinfo = NULL;
        ret = 1;
        goto EXIT;
    }
    memcpy(address, addrinfo->ai_addr, addrinfo->ai_addrlen);

EXIT:
    if (addrinfo != NULL) freeaddrinfo(addrinfo);

    return ret;
}

static int packet_naton(char const *mac_address, char const *type, char const *ifname, struct sockaddr *address)
{
    int ret = 0;
    int i;
    int j;
    char *p;
    unsigned short type_num;
    unsigned mac[6] = {0};
    unsigned if_mac[6] = {0};
    int sock = -1;
    struct ifconf ifc[1];
    char buf[1024];
    struct ifreq ifr[1];

    ((struct sockaddr_ll *)address)->sll_family = AF_PACKET;
    if (type != NULL) {
        type_num = strtoul(type, &p, 0);
        if ((*p != '\0') || (type_num < 0x0600)) {
            ret = 1;
            goto EXIT;
        }
    } else {
        type_num = ETH_P_ALL;
    }
    ((struct sockaddr_ll *)address)->sll_protocol = htons(type_num);
    if ((mac_address != NULL) && (sscanf(mac_address, "%x:%x:%x:%x:%x:%x", mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5) != 6)) {
        ret = 1;
        goto EXIT;
    }
    sock = socket(PF_PACKET, SOCK_DGRAM, 0);
    if (sock == -1) {
        ret = 1;
        goto EXIT;
    }
    ifc->ifc_len = sizeof(buf);
    ifc->ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, ifc) == -1) {
        ret = 1;
        goto EXIT;
    }
    if ((ifname != NULL) && (sscanf(ifname, "%x:%x:%x:%x:%x:%x", if_mac, if_mac + 1, if_mac + 2, if_mac + 3, if_mac + 4, if_mac + 5) != 6)) {
        for (i = 0 ; i < (sizeof(buf) / sizeof(*ifr)) ; i++) {
            if (strcmp(ifname, (ifc->ifc_req + i)->ifr_name) == 0) {
                strcpy(ifr->ifr_name, (ifc->ifc_req + i)->ifr_name);
                if (ioctl(sock, SIOCGIFHWADDR, ifr) == -1) {
                    ret = 1;
                    goto EXIT;
                }
                for (j = 0 ; j < 6 ; j++) {
                    if_mac[j] = ((unsigned char *)(ifr->ifr_hwaddr.sa_data))[j];
                }
                break;
            }
        }
        if (i == (sizeof(buf) / sizeof(*ifr))) {
            ret = 1;
            goto EXIT;
        }
        if(ioctl(sock, SIOCGIFINDEX, ifr) == -1) {
            ret = 1;
            goto EXIT;
        }
        ((struct sockaddr_ll *)address)->sll_ifindex = ifr->ifr_ifindex;
    } else if ((if_mac[0] == 0) && (if_mac[1] == 0) && (if_mac[2] == 0) && (if_mac[3] == 0) && (if_mac[4] == 0) && (if_mac[5] == 0)) {
        ((struct sockaddr_ll *)address)->sll_ifindex = 0;
    } else {
        for (i = 0 ; i < (sizeof(buf) / sizeof(*ifr)) ; i++) {
            strcpy(ifr->ifr_name, (ifc->ifc_req + i)->ifr_name);
            if (ioctl(sock, SIOCGIFHWADDR, ifr) == -1) {
                ret = 1;
                goto EXIT;
            }
            for (j = 0 ; j < 6 ; j++) {
                if (((unsigned char *)(ifr->ifr_hwaddr.sa_data))[j] != if_mac[j]) {
                    break;
                }
            }
            if (j == 6) {
                break;
            }
        }
        if (i == (sizeof(buf) / sizeof(*ifr))) {
            ret = 1;
            goto EXIT;
        }
        if(ioctl(sock, SIOCGIFINDEX, ifr) == -1) {
            ret = 1;
            goto EXIT;
        }
        ((struct sockaddr_ll *)address)->sll_ifindex = ifr->ifr_ifindex;
    }
    ((struct sockaddr_ll *)address)->sll_hatype = 0;
    ((struct sockaddr_ll *)address)->sll_pkttype = 0;
    if ((mac[0] == 0) && (mac[1] == 0) && (mac[2] == 0) && (mac[3] == 0) && (mac[4] == 0) && (mac[5] == 0)) {
        ((struct sockaddr_ll *)address)->sll_halen = 0;
        ((struct sockaddr_ll *)address)->sll_addr[0] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[1] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[2] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[3] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[4] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[5] = 0;
    } else {
        ((struct sockaddr_ll *)address)->sll_halen = 6;
        ((struct sockaddr_ll *)address)->sll_addr[0] = mac[0];
        ((struct sockaddr_ll *)address)->sll_addr[1] = mac[1];
        ((struct sockaddr_ll *)address)->sll_addr[2] = mac[2];
        ((struct sockaddr_ll *)address)->sll_addr[3] = mac[3];
        ((struct sockaddr_ll *)address)->sll_addr[4] = mac[4];
        ((struct sockaddr_ll *)address)->sll_addr[5] = mac[5];
    }

EXIT:
    if (sock != -1) close(sock);

    return ret;
}

static int scan_arg(int argc, char const **argv)
{
    int i;
    int j;
    int k;
    char const *p;
    int ret = 0;
    char c = '\0';

    arg_protocol = PF_INET;
    arg_address = "localhost";
    arg_port = "12345";
    arg_bind_address = NULL;
    arg_bind_port = NULL;
    arg_mac_address = "ff:ff:ff:ff:ff:ff";
    arg_type = "0xffff";
    arg_ifname = "eth0";
    arg_public_key = NULL;
    arg_is_close_half = 0;
    arg_is_silent_enable = 0;

    for (i = 1 ; i < argc ; i++) {
        if (argv[i][0] == '-') {
            c = argv[i][1];
            switch (c) {
            case '4':
                arg_protocol = PF_INET;
                c = '\0';
                break;
            case '6':
                arg_protocol = PF_INET6;
                c = '\0';
                break;
            case '0':
                arg_protocol = PF_PACKET;
                c = '\0';
                break;
            case 'a':
            case 'p':
            case 'b':
            case 'q':
            case 'k':
                break;
            case 'h':
                arg_is_close_half = 1;
                c = '\0';
                break;
            case 's':
                arg_is_silent_enable = 1;
                c = '\0';
                break;
            default:
                ret = 1;
                break;
            }
            if (ret) {
                break;
            }
            continue;
        }
        switch (c) {
        case 'a':
            arg_address = argv[i];
            arg_mac_address = argv[i];
            c = '\0';
            break;
        case 'p':
            arg_port = argv[i];
            arg_type = argv[i];
            c = '\0';
            break;
        case 'b':
            arg_bind_address = argv[i];
            arg_ifname = argv[i];
            c = '\0';
            break;
        case 'q':
            arg_bind_port = argv[i];
            c = '\0';
            break;
        case 'k':
            for (j = 0 ; (j < CIPHER_ED25519_PUBLIC_KEY_SIZE * 2) &&  (argv[i][j] != '\0') ; j++) {
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
        default:
            ret = 1;
            break;
        }
        if (ret) {
            break;
        }
    }

    if (ret) {
        p = strrchr(argv[0], '/');
        if (p == NULL) {
            p = argv[0];
        } else {
            p++;
        }
        printf("%s "
                "[-4 (ipv4) | -6 (ipv6)] "
                    "[-a <destination address>] "
                    "[-p <destination port>] "
                    "[-b <bind address>] "
                    "[-q <bind port>] "
                    "[-h (half close)] "
                    "[-s (silent)]\n"
            "%s "
                "-0 (raw) "
                    "[-a <destination address>] "
                    "[-p <ether type>] "
                    "[-b <interface name>] "
                    "[-h (half close)] "
                    "[-s (silent)]\n",
            p, p);
        printf("%s (C) 2024 akinoyonika\n", p);
        printf("%s\n", espress_stream_copyright());
    }

    return ret;
}

struct session_attr {
    void *sid;
    volatile int is_running;
    unsigned char message[CONNECT_MESSAGE_SIZE];
    unsigned message_size;
    int event;
};
struct session_attr session_attr[1];

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
    int ret;
    unsigned char *message = ((struct session_attr *)param)->message;
    unsigned *message_size = &(((struct session_attr *)param)->message_size);
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
    unsigned char *message = ((struct session_attr *)param)->message;
    unsigned *message_size = &(((struct session_attr *)param)->message_size);
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

static int conn(struct session_attr *attr,
    struct sockaddr_storage const *address,
    unsigned timeout)
{
    int i;
    unsigned t;
    volatile int *is_running = &(attr->is_running);
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

            if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
                break;
            }
        }
    }

    return i;
}

static int flush(struct session_attr *attr, unsigned timeout)
{
    int i;
    unsigned u;
    unsigned v;
    unsigned t;
    volatile int *is_running = &(attr->is_running);
    void **sid = &(attr->sid);
    unsigned start_time;

    espress_stream_flush(*sid, 0);
    for (i = -1, u = espress_stream_buffered_send_size(*sid, NULL), start_time = espress_stream_time(), t = 0 ;
        *is_running & (i == -1) ;
        u = v) {
        t = timeout - t;
        i = espress_stream_flush(*sid, t);

        v = espress_stream_buffered_send_size(*sid, NULL);
        if (u != v) {
            start_time = espress_stream_time();
        }

        if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
            break;
        }
    }

    return i;
}

static void sub_wait(void *param, unsigned events)
{
    void **sid = &(((struct session_attr *)param)->sid);
    int *event = &(((struct session_attr *)param)->event);
    espress_stream_wait_any(*sid, events, 0xffffffff);
    set_eventfd(*event);
}

static void *s_wait(void *param)
{
    sub_wait(param, ESPRESS_STREAM_EVENT_SEND);
    return NULL;
}

static void *r_wait(void *param)
{
    sub_wait(param, ESPRESS_STREAM_EVENT_RECV);
    return NULL;
}

static void *sr_wait(void *param)
{
    sub_wait(param, ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV);
    return NULL;
}

static void wait(struct session_attr *attr,
    int is_stdin, int is_stdout, int is_sidin, int is_sidout,
    unsigned timeout)
{
    int i = -1;
    void **sid = &(attr->sid);
    int *event = &(attr->event);
    pthread_t sub_wait_thread;
    struct pollfd fds[3];
    nfds_t nfds = 0;

    if (is_stdin) {
        fds[nfds].fd = STDIN_FILENO;
        fds[nfds].events = POLLIN;
        fds[nfds].revents = 0;
        nfds++;
    }

    if (is_stdout) {
        fds[nfds].fd = STDOUT_FILENO;
        fds[nfds].events = POLLOUT;
        fds[nfds].revents = 0;
        nfds++;
    }

    if (is_sidin || is_sidout) {
        *event = eventfd(0, 0);
        if (0 <= *event) {
            if (is_sidin && is_sidout) {
                i = pthread_create(&sub_wait_thread, NULL, sr_wait, attr);
            } else if (is_sidin) {
                i = pthread_create(&sub_wait_thread, NULL, r_wait, attr);
            } else if (is_sidout) {
                i = pthread_create(&sub_wait_thread, NULL, s_wait, attr);
            }
            if (i == 0) {
                fds[nfds].fd = *event;
                fds[nfds].events = POLLIN;
                fds[nfds].revents = 0;
                nfds++;
            }
        }
    }

    poll(fds, nfds, timeout / 1000);

    if (is_sidin || is_sidout) {
        if (0 <= *event) {
            if (i == 0) {
                espress_stream_wait_break(*sid);
                wait_eventfd(*event);
                pthread_join(sub_wait_thread, NULL);
                espress_stream_wait_cancel_break(*sid);
            }
            close(*event);
        }
    }
}

static void cat(struct session_attr *attr)
{
    int i;
    ssize_t st;
    unsigned u;
    unsigned v;
    volatile int *is_running = &(attr->is_running);
    void **sid = &(attr->sid);
    unsigned char read_buf[BUFFER_SIZE];
    unsigned read_buf_size = 0;
    unsigned char recv_buf[BUFFER_SIZE];
    unsigned recv_buf_size = 0;
    unsigned char *write_buf = recv_buf;
#   define write_buf_size recv_buf_size
    unsigned char *send_buf = read_buf;
#   define send_buf_size read_buf_size
    int is_stdin_open;
    int is_stdout_open;
    int is_sidin_open;
    int is_sidout_open;
    unsigned stdout_tm;
    unsigned sidout_tm;

    for (is_stdin_open = 1, is_stdout_open = 1, is_sidin_open = 1, is_sidout_open = 1, stdout_tm = sidout_tm = espress_stream_time() ;
        *is_running && (is_stdin_open || is_stdout_open || is_sidin_open || is_sidout_open) ;
        ) {
        v = espress_stream_time();
        if (is_stdout_open && (0 < write_buf_size)) {
            u = (v - stdout_tm) & 0xffffffff;
            u = (u < TIMEOUT) ? (TIMEOUT - u) : TIMEOUT;
        } else {
            u = 0xffffffff;
        }
        if (is_sidout_open && (0 < send_buf_size)) {
            v = (v - sidout_tm) & 0xffffffff;
            v = (v < TIMEOUT) ? (TIMEOUT - v) : TIMEOUT;
        } else {
            v = 0xffffffff;
        }
        wait(attr,
            is_stdin_open && (read_buf_size == 0),
            is_stdout_open && (0 < write_buf_size),
            is_sidin_open && (recv_buf_size == 0),
            is_sidout_open && (0 < send_buf_size),
            *is_running ? ((u < v) ? u : v) : 0);

        if (is_stdin_open) {
            if (read_buf_size == 0) {
                st = read(STDIN_FILENO, read_buf, BUFFER_SIZE);
                if (0 < st) {
                    read_buf_size = st;
                } else if (st == 0) {
                    close(STDIN_FILENO);
                    is_stdin_open = 0;
                    if (send_buf_size == 0) {
                        i = flush(attr, TIMEOUT);
                        espress_stream_disconnect(*sid);
                        espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
                        is_sidout_open = 0;
                        if (0 < i) {
                            break;
                        }
                    }
                } else if (errno != EAGAIN) {
                    break;
                }
            } else {
                st = read(STDIN_FILENO, NULL, 0);
                if ((st == -1) && (errno != EAGAIN)) {
                    break;
                }
            }
        }

        if (is_sidin_open) {
            if (recv_buf_size == 0) {
                u = BUFFER_SIZE;
                i = espress_stream_recv(*sid, recv_buf, &u, 0);
                if (i == 0) {
                    if (0 < u) {
                        recv_buf_size = u;
                    } else {
                        is_sidin_open = 0;
                        if (write_buf_size == 0) {
                            close(STDOUT_FILENO);
                            is_stdout_open = 0;
                        }
                        if (!arg_is_close_half && is_stdin_open)
                        {
                            close(STDIN_FILENO);
                            is_stdin_open = 0;
                            if (send_buf_size == 0) {
                                i = flush(attr, TIMEOUT);
                                espress_stream_disconnect(*sid);
                                espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
                                is_sidout_open = 0;
                                if (0 < i) {
                                    break;
                                }
                            }
                        }
                    }
                } else if (0 < i) {
                    break;
                }
            } else {
                u = 1;
                i = espress_stream_recv(*sid, NULL, &u, 0);
                if (0 < i) {
                    break;
                }
            }
        }

        if (is_stdout_open) {
            if (0 < write_buf_size) {
                st = write(STDOUT_FILENO, write_buf, write_buf_size);
                if (0 < st) {
                    write_buf += st;
                    write_buf_size -= st;
                    if (write_buf_size == 0) {
                        write_buf = recv_buf;
                        if (is_sidin_open) {
                            //fsync(STDOUT_FILENO);
                        } else {
                            close(STDOUT_FILENO);
                            is_stdout_open = 0;
                        }
                    }
                    stdout_tm = espress_stream_time();
                } else if (errno != EAGAIN) {
                    break;
                } else {
                    if (TIMEOUT <= ((espress_stream_time() - stdout_tm) & 0xffffffff)) {
                        break;
                    }
                }
            } else {
                st = write(STDOUT_FILENO, NULL, 0);
                if ((st == -1) && (errno != EAGAIN)) {
                    break;
                }
                stdout_tm = espress_stream_time();
            }
        }

        if (is_sidout_open) {
            if (0 < send_buf_size) {
                u = send_buf_size;
                i = espress_stream_send(*sid, send_buf, &u, 0);
                if (i == 0) {
                    send_buf += u;
                    send_buf_size -= u;
                    if (send_buf_size == 0) {
                        send_buf = read_buf;
                        if (is_stdin_open) {
                            espress_stream_flush(*sid, 0);
                        } else {
                            i = flush(attr, TIMEOUT);
                            espress_stream_disconnect(*sid);
                            espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
                            is_sidout_open = 0;
                            if (0 < i) {
                                break;
                            }
                        }
                    }
                    sidout_tm = espress_stream_time();
                } else if (0 < i) {
                    break;
                } else {
                    if (TIMEOUT <= ((espress_stream_time() - sidout_tm) & 0xffffffff)) {
                        break;
                    }
                }
            } else {
                u = 1;
                i = espress_stream_send(*sid, NULL, &u, 0);
                if (0 < i) {
                    break;
                }
                sidout_tm = espress_stream_time();
            }
        }
    }

    if (is_stdin_open) {
        close(STDIN_FILENO);
    }
    if (is_stdout_open) {
        close(STDOUT_FILENO);
    }
    if (is_sidin_open) {
        ;
    }
    if (is_sidout_open) {
        espress_stream_disconnect(*sid);
        espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
    }
}

static int sige = -1;

static void sigh(int signum)
{
    set_eventfd(sige);
}

static void *sigt(void *param)
{
    wait_eventfd(sige);
    session_attr->is_running = 0;
    if (session_attr->sid != NULL) {
        espress_stream_wait_break(session_attr->sid);
    }
    return param;
}

int main(int argc, char const **argv)
{
    int ret = 0;
    int i;
    struct sockaddr_storage address[1] = {0};
    struct sockaddr_storage src_address[1] = {0};
    struct session_attr *attr = session_attr;
    struct espress_stream_session_ex_param ex_param[1] = {{
        send_connect_payload, NULL,
        recv_connect_payload, NULL,
        NULL, NULL,
        NULL, NULL
    }};
    pthread_t signal_thread;

    i = scan_arg(argc, argv);
    if (i) {
        ret = 1;
        goto SCAN_ARG_FAIL_EXIT;
    }

    if (!arg_is_silent_enable) {
        switch (arg_protocol) {
        case PF_PACKET:
            printf("protocol: raw\n");
            printf("destination address: %s\n", arg_mac_address);
            printf("ether type: %s\n", arg_type);
            printf("interface name: %s\n", arg_ifname);
            break;
        case PF_INET6:
        case PF_INET:
        default:
            printf("protocol: %s\n", (arg_protocol == PF_INET6) ? "ipv6" : "ipv4");
            printf("destination address: %s\n", arg_address);
            printf("destination port: %s\n", arg_port);
            printf("bind address: %s\n", (arg_bind_address == NULL) ? "(any)" : arg_bind_address);
            printf("bind port: %s\n", (arg_bind_port == NULL) ? "(any)" : arg_bind_port);
            break;
        }
        printf("destination public key: ");
        if (arg_public_key == NULL) {
            printf("(none)");
        } else {
            for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
                printf("%02x", arg_public_key[i]);
            }
        }
        printf("\n");
        printf("close: %s\n", arg_is_close_half ? "half" : "full");
    }

    if (arg_protocol == PF_PACKET) {
        i = packet_naton(arg_mac_address, arg_type, arg_ifname, (struct sockaddr *)address);
        if (i) {
            ret = 1;
            fprintf(stderr, "error: invalid destination_address/ether_type/interface_name\n");
            goto SCAN_ARG_FAIL_EXIT;
        }
        i = packet_naton(NULL, arg_type, NULL, (struct sockaddr *)src_address);
        if (i) {
            ret = 1;
            fprintf(stderr, "error: invalid ether_type\n");
            goto SCAN_ARG_FAIL_EXIT;
        }
    } else {
        i = inet_naton(arg_protocol, arg_address, arg_port, (struct sockaddr *)address);
        if (i) {
            ret = 1;
            fprintf(stderr, "error: invalid destination_address/destination_port\n");
            goto SCAN_ARG_FAIL_EXIT;
        }
        i = inet_naton(arg_protocol, arg_bind_address, arg_bind_port, (struct sockaddr *)src_address);
        if (i) {
            ret = 1;
            fprintf(stderr, "error: invalid bind_address/bind_port\n");
            goto SCAN_ARG_FAIL_EXIT;
        }
    }

    i = espress_stream_begin();
    if (i != 0) {
        ret = 1;
        fprintf(stderr, "error: can't begin stream.\n");
        goto BEGIN_STREAM_FAIL_EXIT;
    }

    i = espress_stream_begin_interface(&ifd, src_address);
    if (i != 0) {
        ret = 1;
        fprintf(stderr, "error: can't begin interface.\n");
        goto BEGIN_INTERFACE_FAIL_EXIT;
    }

    sige = eventfd(0, 0);
    if (sige == -1) {
        ret = 1;
        fprintf(stderr, "error: can't create eventfd.\n");
        goto CREATE_EVENTFD_FAIL_EXIT;
    }

    i = pthread_create(&signal_thread, NULL, sigt, NULL);
    if (i != 0) {
        ret = 1;
        fprintf(stderr, "error: can't create thread.\n");
        goto CREATE_THREAD_FAIL_EXIT;
    }

    attr->is_running = 1;
    ex_param->send_connect_payload_param = attr;
    ex_param->recv_connect_payload_param = attr;
    i = espress_stream_begin_session(&(attr->sid), ifd, 0, ex_param);
    if (i != 0) {
        ret = 1;
        fprintf(stderr, "error: can't begin session.\n");
        goto BEGIN_SESSION_FAIL_EXIT;
    }

    signal(SIGINT, sigh);
    signal(SIGTERM, sigh);

    i = conn(attr, address, attr->is_running ? TIMEOUT : 0);
    if (i != 0) {
        fprintf(stderr, "error: fail to connect.\n");
        ret = 1;
        goto CONN_FAIL_EXIT;
    }

    if (!arg_is_silent_enable) {
        printf("CONNECT\n");
        fflush(stdout);
    }

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);

    cat(attr);

CONN_FAIL_EXIT:
    signal(SIGTERM, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    espress_stream_wait_cancel_break(attr->sid);
    espress_stream_end_session(attr->sid);
    attr->sid = NULL;
BEGIN_SESSION_FAIL_EXIT:
    set_eventfd(sige);
    pthread_join(signal_thread, NULL);
CREATE_THREAD_FAIL_EXIT:
    close(sige);
CREATE_EVENTFD_FAIL_EXIT:
    espress_stream_end_interface(ifd);
BEGIN_INTERFACE_FAIL_EXIT:
    espress_stream_end();
BEGIN_STREAM_FAIL_EXIT:
SCAN_ARG_FAIL_EXIT:

    return ret;
}
```

</details>

<details><summary><code>arch/linux/app/es_ccat-es_scat/es_scat/src/main.c</code></summary>

```
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include "espress_stream/api.h"
#include "cipher_api.h"
#include "cert.h"

static unsigned char const secret_key[] = SECRET_KEY;
static unsigned char const public_key[] = PUBLIC_KEY;

#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (8 * 1024)
#define CONNECT_MESSAGE_SIZE (64)

#define set_eventfd(fd) do{uint64_t u = 1; while (write(fd, &u, sizeof(u)) != sizeof(u));}while (0)
#define wait_eventfd(fd) do{uint64_t u = 1; while (read(fd, &u, sizeof(u)) != sizeof(u));}while (0) 

static void *ifd;
static int arg_protocol;
static char const *arg_address;
static char const *arg_port;
static char const *arg_ifname;
static char const *arg_type;
static int arg_is_close_half;
static int arg_is_silent_enable;

static int inet_naton(int protocol, char const *name, char const *port, struct sockaddr *address)
{
    int ret = 0;
    struct addrinfo hint[1] = {0};
    struct addrinfo *addrinfo = NULL;

    hint->ai_family = protocol;
    hint->ai_socktype = SOCK_DGRAM;
    if (name == NULL) {
        hint->ai_flags = AI_PASSIVE;
    }
    if (port == NULL) {
        port = "0";
    }
    ret = getaddrinfo(name, port, hint, &addrinfo);
    if (ret != 0) {
        addrinfo = NULL;
        ret = 1;
        goto EXIT;
    }
    memcpy(address, addrinfo->ai_addr, addrinfo->ai_addrlen);

EXIT:
    if (addrinfo != NULL) freeaddrinfo(addrinfo);

    return ret;
}

static int packet_naton(char const *mac_address, char const *type, char const *ifname, struct sockaddr *address)
{
    int ret = 0;
    int i;
    int j;
    char *p;
    unsigned short type_num;
    unsigned mac[6] = {0};
    unsigned if_mac[6] = {0};
    int sock = -1;
    struct ifconf ifc[1];
    char buf[1024];
    struct ifreq ifr[1];

    ((struct sockaddr_ll *)address)->sll_family = AF_PACKET;
    if (type != NULL) {
        type_num = strtoul(type, &p, 0);
        if ((*p != '\0') || (type_num < 0x0600)) {
            ret = 1;
            goto EXIT;
        }
    } else {
        type_num = ETH_P_ALL;
    }
    ((struct sockaddr_ll *)address)->sll_protocol = htons(type_num);
    if ((mac_address != NULL) && (sscanf(mac_address, "%x:%x:%x:%x:%x:%x", mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5) != 6)) {
        ret = 1;
        goto EXIT;
    }
    sock = socket(PF_PACKET, SOCK_DGRAM, 0);
    if (sock == -1) {
        ret = 1;
        goto EXIT;
    }
    ifc->ifc_len = sizeof(buf);
    ifc->ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, ifc) == -1) {
        ret = 1;
        goto EXIT;
    }
    if ((ifname != NULL) && (sscanf(ifname, "%x:%x:%x:%x:%x:%x", if_mac, if_mac + 1, if_mac + 2, if_mac + 3, if_mac + 4, if_mac + 5) != 6)) {
        for (i = 0 ; i < (sizeof(buf) / sizeof(*ifr)) ; i++) {
            if (strcmp(ifname, (ifc->ifc_req + i)->ifr_name) == 0) {
                strcpy(ifr->ifr_name, (ifc->ifc_req + i)->ifr_name);
                if (ioctl(sock, SIOCGIFHWADDR, ifr) == -1) {
                    ret = 1;
                    goto EXIT;
                }
                for (j = 0 ; j < 6 ; j++) {
                    if_mac[j] = ((unsigned char *)(ifr->ifr_hwaddr.sa_data))[j];
                }
                break;
            }
        }
        if (i == (sizeof(buf) / sizeof(*ifr))) {
            ret = 1;
            goto EXIT;
        }
        if(ioctl(sock, SIOCGIFINDEX, ifr) == -1) {
            ret = 1;
            goto EXIT;
        }
        ((struct sockaddr_ll *)address)->sll_ifindex = ifr->ifr_ifindex;
    } else if ((if_mac[0] == 0) && (if_mac[1] == 0) && (if_mac[2] == 0) && (if_mac[3] == 0) && (if_mac[4] == 0) && (if_mac[5] == 0)) {
        ((struct sockaddr_ll *)address)->sll_ifindex = 0;
    } else {
        for (i = 0 ; i < (sizeof(buf) / sizeof(*ifr)) ; i++) {
            strcpy(ifr->ifr_name, (ifc->ifc_req + i)->ifr_name);
            if (ioctl(sock, SIOCGIFHWADDR, ifr) == -1) {
                ret = 1;
                goto EXIT;
            }
            for (j = 0 ; j < 6 ; j++) {
                if (((unsigned char *)(ifr->ifr_hwaddr.sa_data))[j] != if_mac[j]) {
                    break;
                }
            }
            if (j == 6) {
                break;
            }
        }
        if (i == (sizeof(buf) / sizeof(*ifr))) {
            ret = 1;
            goto EXIT;
        }
        if(ioctl(sock, SIOCGIFINDEX, ifr) == -1) {
            ret = 1;
            goto EXIT;
        }
        ((struct sockaddr_ll *)address)->sll_ifindex = ifr->ifr_ifindex;
    }
    ((struct sockaddr_ll *)address)->sll_hatype = 0;
    ((struct sockaddr_ll *)address)->sll_pkttype = 0;
    if ((mac[0] == 0) && (mac[1] == 0) && (mac[2] == 0) && (mac[3] == 0) && (mac[4] == 0) && (mac[5] == 0)) {
        ((struct sockaddr_ll *)address)->sll_halen = 0;
        ((struct sockaddr_ll *)address)->sll_addr[0] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[1] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[2] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[3] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[4] = 0;
        ((struct sockaddr_ll *)address)->sll_addr[5] = 0;
    } else {
        ((struct sockaddr_ll *)address)->sll_halen = 6;
        ((struct sockaddr_ll *)address)->sll_addr[0] = mac[0];
        ((struct sockaddr_ll *)address)->sll_addr[1] = mac[1];
        ((struct sockaddr_ll *)address)->sll_addr[2] = mac[2];
        ((struct sockaddr_ll *)address)->sll_addr[3] = mac[3];
        ((struct sockaddr_ll *)address)->sll_addr[4] = mac[4];
        ((struct sockaddr_ll *)address)->sll_addr[5] = mac[5];
    }

EXIT:
    if (sock != -1) close(sock);

    return ret;
}

static int scan_arg(int argc, char const **argv)
{
    int i;
    char const *p;
    int ret = 0;
    char c = '\0';

    arg_protocol = PF_INET;
    arg_address = NULL;
    arg_port = "12345";
    arg_ifname = NULL;
    arg_type = NULL;
    arg_is_close_half = 0;
    arg_is_silent_enable = 0;

    for (i = 1 ; i < argc ; i++) {
        if (argv[i][0] == '-') {
            c = argv[i][1];
            switch (c) {
            case '4':
                arg_protocol = PF_INET;
                c = '\0';
                break;
            case '6':
                arg_protocol = PF_INET6;
                c = '\0';
                break;
            case '0':
                arg_protocol = PF_PACKET;
                c = '\0';
                break;
            case 'a':
            case 'p':
                break;
            case 'h':
                arg_is_close_half = 1;
                c = '\0';
                break;
            case 's':
                arg_is_silent_enable = 1;
                c = '\0';
                break;
            default:
                ret = 1;
                break;
            }
            if (ret) {
                break;
            }
            continue;
        }
        switch (c) {
        case 'a':
            arg_address = argv[i];
            arg_ifname = argv[i];
            c = '\0';
            break;
        case 'p':
            arg_port = argv[i];
            arg_type = argv[i];
            c = '\0';
            break;
        default:
            ret = 1;
            break;
        }
        if (ret) {
            break;
        }
    }

    if (ret) {
        p = strrchr(argv[0], '/');
        if (p == NULL) {
            p = argv[0];
        } else {
            p++;
        }
        printf("%s "
                "[-4 (ipv4) | -6 (ipv6)] "
                    "[-a <bind address>] "
                    "[-p <bind port>] "
                    "[-h (half close)] "
                    "[-s (silent)]\n"
            "%s "
                "-0 (raw) "
                    "[-a <interface name>] "
                    "[-p <ether type>] "
                    "[-h (half close)] "
                    "[-s (silent)]\n",
            p, p);
        printf("%s (C) 2024 akinoyonika\n", p);
        printf("%s\n", espress_stream_copyright());
    }

    return ret;
}

struct session_attr {
    void *sid;
    volatile int is_running;
    unsigned char message[CONNECT_MESSAGE_SIZE];
    unsigned message_size;
    int event;
};
struct session_attr session_attr[1];

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
    int ret;
    unsigned char *message = ((struct session_attr *)param)->message;
    unsigned message_size = ((struct session_attr *)param)->message_size;
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
    unsigned char *message = ((struct session_attr *)param)->message;
    unsigned *message_size = &(((struct session_attr *)param)->message_size);
    *message_size = CONNECT_MESSAGE_SIZE;
    return callback(sid, message, message_size);
}

static int accpt(struct session_attr *attr, unsigned timeout)
{
    int i;
    unsigned t;
    volatile int *is_running = &(attr->is_running);
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

    for (i = -1, t = 0 ; *is_running && (i == -1) ;) {
        t = timeout - t;
        i = espress_stream_connect_detect(*sid, t);
        if (0 < i) {
            ex_param->send_connect_payload_param = attr;
            ex_param->recv_connect_payload_param = attr;
            i = espress_stream_begin_session(&new_sid, ifd, 1, ex_param);
            if (i == 0) {
                old_sid = *sid;
                *sid = new_sid;
                espress_stream_end_session(old_sid);
            }
        }

        if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
            break;
        }
    }

    return i;
}

static int flush(struct session_attr *attr, unsigned timeout)
{
    int i;
    unsigned u;
    unsigned v;
    unsigned t;
    volatile int *is_running = &(attr->is_running);
    void **sid = &(attr->sid);
    unsigned start_time;

    espress_stream_flush(*sid, 0);
    for (i = -1, u = espress_stream_buffered_send_size(*sid, NULL), start_time = espress_stream_time(), t = 0 ;
        *is_running & (i == -1) ;
        u = v) {
        t = timeout - t;
        i = espress_stream_flush(*sid, t);

        v = espress_stream_buffered_send_size(*sid, NULL);
        if (u != v) {
            start_time = espress_stream_time();
        }

        if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
            break;
        }
    }

    return i;
}

static void sub_wait(void *param, unsigned events)
{
    void **sid = &(((struct session_attr *)param)->sid);
    int *event = &(((struct session_attr *)param)->event);
    espress_stream_wait_any(*sid, events, 0xffffffff);
    set_eventfd(*event);
}

static void *s_wait(void *param)
{
    sub_wait(param, ESPRESS_STREAM_EVENT_SEND);
    return NULL;
}

static void *r_wait(void *param)
{
    sub_wait(param, ESPRESS_STREAM_EVENT_RECV);
    return NULL;
}

static void *sr_wait(void *param)
{
    sub_wait(param, ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV);
    return NULL;
}

static void wait(struct session_attr *attr,
    int is_stdin, int is_stdout, int is_sidin, int is_sidout,
    unsigned timeout)
{
    int i = -1;
    void **sid = &(attr->sid);
    int *event = &(attr->event);
    pthread_t sub_wait_thread;
    struct pollfd fds[3];
    nfds_t nfds = 0;

    if (is_stdin) {
        fds[nfds].fd = STDIN_FILENO;
        fds[nfds].events = POLLIN;
        fds[nfds].revents = 0;
        nfds++;
    }

    if (is_stdout) {
        fds[nfds].fd = STDOUT_FILENO;
        fds[nfds].events = POLLOUT;
        fds[nfds].revents = 0;
        nfds++;
    }

    if (is_sidin || is_sidout) {
        *event = eventfd(0, 0);
        if (0 <= *event) {
            if (is_sidin && is_sidout) {
                i = pthread_create(&sub_wait_thread, NULL, sr_wait, attr);
            } else if (is_sidin) {
                i = pthread_create(&sub_wait_thread, NULL, r_wait, attr);
            } else if (is_sidout) {
                i = pthread_create(&sub_wait_thread, NULL, s_wait, attr);
            }
            if (i == 0) {
                fds[nfds].fd = *event;
                fds[nfds].events = POLLIN;
                fds[nfds].revents = 0;
                nfds++;
            }
        }
    }

    poll(fds, nfds, timeout / 1000);

    if (is_sidin || is_sidout) {
        if (0 <= *event) {
            if (i == 0) {
                espress_stream_wait_break(*sid);
                wait_eventfd(*event);
                pthread_join(sub_wait_thread, NULL);
                espress_stream_wait_cancel_break(*sid);
            }
            close(*event);
        }
    }
}

static void cat(struct session_attr *attr)
{
    int i;
    ssize_t st;
    unsigned u;
    unsigned v;
    volatile int *is_running = &(attr->is_running);
    void **sid = &(attr->sid);
    unsigned char read_buf[BUFFER_SIZE];
    unsigned read_buf_size = 0;
    unsigned char recv_buf[BUFFER_SIZE];
    unsigned recv_buf_size = 0;
    unsigned char *write_buf = recv_buf;
#   define write_buf_size recv_buf_size
    unsigned char *send_buf = read_buf;
#   define send_buf_size read_buf_size
    int is_stdin_open;
    int is_stdout_open;
    int is_sidin_open;
    int is_sidout_open;
    unsigned stdout_tm;
    unsigned sidout_tm;

    for (is_stdin_open = 1, is_stdout_open = 1, is_sidin_open = 1, is_sidout_open = 1, stdout_tm = sidout_tm = espress_stream_time() ;
        *is_running && (is_stdin_open || is_stdout_open || is_sidin_open || is_sidout_open) ;
        ) {
        v = espress_stream_time();
        if (is_stdout_open && (0 < write_buf_size)) {
            u = (v - stdout_tm) & 0xffffffff;
            u = (u < TIMEOUT) ? (TIMEOUT - u) : TIMEOUT;
        } else {
            u = 0xffffffff;
        }
        if (is_sidout_open && (0 < send_buf_size)) {
            v = (v - sidout_tm) & 0xffffffff;
            v = (v < TIMEOUT) ? (TIMEOUT - v) : TIMEOUT;
        } else {
            v = 0xffffffff;
        }
        wait(attr,
            is_stdin_open && (read_buf_size == 0),
            is_stdout_open && (0 < write_buf_size),
            is_sidin_open && (recv_buf_size == 0),
            is_sidout_open && (0 < send_buf_size),
            *is_running ? ((u < v) ? u : v) : 0);

        if (is_stdin_open) {
            if (read_buf_size == 0) {
                st = read(STDIN_FILENO, read_buf, BUFFER_SIZE);
                if (0 < st) {
                    read_buf_size = st;
                } else if (st == 0) {
                    close(STDIN_FILENO);
                    is_stdin_open = 0;
                    if (send_buf_size == 0) {
                        i = flush(attr, TIMEOUT);
                        espress_stream_disconnect(*sid);
                        espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
                        is_sidout_open = 0;
                        if (0 < i) {
                            break;
                        }
                    }
                } else if (errno != EAGAIN) {
                    break;
                }
            } else {
                st = read(STDIN_FILENO, NULL, 0);
                if ((st == -1) && (errno != EAGAIN)) {
                    break;
                }
            }
        }

        if (is_sidin_open) {
            if (recv_buf_size == 0) {
                u = BUFFER_SIZE;
                i = espress_stream_recv(*sid, recv_buf, &u, 0);
                if (i == 0) {
                    if (0 < u) {
                        recv_buf_size = u;
                    } else {
                        is_sidin_open = 0;
                        if (write_buf_size == 0) {
                            close(STDOUT_FILENO);
                            is_stdout_open = 0;
                        }
                        if (!arg_is_close_half && is_stdin_open)
                        {
                            close(STDIN_FILENO);
                            is_stdin_open = 0;
                            if (send_buf_size == 0) {
                                i = flush(attr, TIMEOUT);
                                espress_stream_disconnect(*sid);
                                espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
                                is_sidout_open = 0;
                                if (0 < i) {
                                    break;
                                }
                            }
                        }
                    }
                } else if (0 < i) {
                    break;
                }
            } else {
                u = 1;
                i = espress_stream_recv(*sid, NULL, &u, 0);
                if (0 < i) {
                    break;
                }
            }
        }

        if (is_stdout_open) {
            if (0 < write_buf_size) {
                st = write(STDOUT_FILENO, write_buf, write_buf_size);
                if (0 < st) {
                    write_buf += st;
                    write_buf_size -= st;
                    if (write_buf_size == 0) {
                        write_buf = recv_buf;
                        if (is_sidin_open) {
                            //fsync(STDOUT_FILENO);
                        } else {
                            close(STDOUT_FILENO);
                            is_stdout_open = 0;
                        }
                    }
                    stdout_tm = espress_stream_time();
                } else if (errno != EAGAIN) {
                    break;
                } else {
                    if (TIMEOUT <= ((espress_stream_time() - stdout_tm) & 0xffffffff)) {
                        break;
                    }
                }
            } else {
                st = write(STDOUT_FILENO, NULL, 0);
                if ((st == -1) && (errno != EAGAIN)) {
                    break;
                }
                stdout_tm = espress_stream_time();
            }
        }

        if (is_sidout_open) {
            if (0 < send_buf_size) {
                u = send_buf_size;
                i = espress_stream_send(*sid, send_buf, &u, 0);
                if (i == 0) {
                    send_buf += u;
                    send_buf_size -= u;
                    if (send_buf_size == 0) {
                        send_buf = read_buf;
                        if (is_stdin_open) {
                            espress_stream_flush(*sid, 0);
                        } else {
                            i = flush(attr, TIMEOUT);
                            espress_stream_disconnect(*sid);
                            espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
                            is_sidout_open = 0;
                            if (0 < i) {
                                break;
                            }
                        }
                    }
                    sidout_tm = espress_stream_time();
                } else if (0 < i) {
                    break;
                } else {
                    if (TIMEOUT <= ((espress_stream_time() - sidout_tm) & 0xffffffff)) {
                        break;
                    }
                }
            } else {
                u = 1;
                i = espress_stream_send(*sid, NULL, &u, 0);
                if (0 < i) {
                    break;
                }
                sidout_tm = espress_stream_time();
            }
        }
    }

    if (is_stdin_open) {
        close(STDIN_FILENO);
    }
    if (is_stdout_open) {
        close(STDOUT_FILENO);
    }
    if (is_sidin_open) {
        ;
    }
    if (is_sidout_open) {
        espress_stream_disconnect(*sid);
        espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
    }
}

static int sige = -1;

static void sigh(int signum)
{
    set_eventfd(sige);
}

static void *sigt(void *param)
{
    wait_eventfd(sige);
    session_attr->is_running = 0;
    if (session_attr->sid != NULL) {
        espress_stream_wait_break(session_attr->sid);
    }
    return param;
}

int main(int argc, char const **argv)
{
    int ret = 0;
    int i;
    struct sockaddr_storage address[1] = {0};
    struct session_attr *attr = session_attr;
    struct espress_stream_session_ex_param ex_param[1] = {{
        send_connect_payload, NULL,
        recv_connect_payload, NULL,
        NULL, NULL,
        NULL, NULL
    }};
    pthread_t signal_thread;

    i = scan_arg(argc, argv);
    if (i) {
        ret = 1;
        goto SCAN_ARG_FAIL_EXIT;
    }

    if (!arg_is_silent_enable) {
        printf("public key: ");
        for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
            printf("%02x", public_key[i]);
        }
        printf("\n");
        switch (arg_protocol) {
        case PF_PACKET:
            printf("protocol: raw\n");
            printf("interface name: %s\n", (arg_ifname == NULL) ? "(any)" : arg_ifname);
            printf("ether type: %s\n", (arg_type == NULL) ? "(any)" : arg_type);
            break;
        case PF_INET6:
        case PF_INET:
        default:
            printf("protocol: %s\n", (arg_protocol == PF_INET6) ? "ipv6" : "ipv4");
            printf("bind address: %s\n", (arg_address == NULL) ? "(any)" : arg_address);
            printf("bind port: %s\n", arg_port);
            break;
        }
        printf("close: %s\n", arg_is_close_half ? "half" : "full");
    }

    if (arg_protocol == PF_PACKET) {
        i = packet_naton(NULL, arg_type, arg_ifname, (struct sockaddr *)address);
        if (i) {
            ret = 1;
            fprintf(stderr, "error: invalid bind_ether_type/bind_interface_name\n");
            goto SCAN_ARG_FAIL_EXIT;
        }
    } else {
        i = inet_naton(arg_protocol, arg_address, arg_port, (struct sockaddr *)address);
        if (i) {
            ret = 1;
            fprintf(stderr, "error: invalid bind_address/bind_port\n");
            goto SCAN_ARG_FAIL_EXIT;
        }
    }

    i = espress_stream_begin();
    if (i != 0) {
        ret = 1;
        fprintf(stderr, "error: can't begin stream.\n");
        goto BEGIN_STREAM_FAIL_EXIT;
    }

    i = espress_stream_begin_interface(&ifd, address);
    if (i != 0) {
        ret = 1;
        fprintf(stderr, "error: can't begin interface.\n");
        goto BEGIN_INTERFACE_FAIL_EXIT;
    }

    sige = eventfd(0, 0);
    if (sige == -1) {
        ret = 1;
        fprintf(stderr, "error: can't create eventfd.\n");
        goto CREATE_EVENTFD_FAIL_EXIT;
    }

    i = pthread_create(&signal_thread, NULL, sigt, NULL);
    if (i != 0) {
        ret = 1;
        fprintf(stderr, "error: can't create thread.\n");
        goto CREATE_THREAD_FAIL_EXIT;
    }

    attr->is_running = 1;
    ex_param->send_connect_payload_param = attr;
    ex_param->recv_connect_payload_param = attr;
    i = espress_stream_begin_session(&(attr->sid), ifd, 1, ex_param);
    if (i != 0) {
        fprintf(stderr, "error: can't begin session.\n");
        ret = 1;
        goto BEGIN_SESSION_FAIL_EXIT;
    }

    signal(SIGINT, sigh);
    signal(SIGTERM, sigh);

    i = accpt(attr, attr->is_running ? TIMEOUT : 0);
    if (i != 0) {
        fprintf(stderr, "error: no acceptable connection.\n");
        ret = 1;
        goto ACCEPT_FAIL_EXIT;
    }

    if (!arg_is_silent_enable) {
        printf("CONNECT\n");
        fflush(stdout);
    }

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);

    cat(attr);

ACCEPT_FAIL_EXIT:
    signal(SIGTERM, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    espress_stream_wait_cancel_break(attr->sid);
    espress_stream_end_session(attr->sid);
    attr->sid = NULL;
BEGIN_SESSION_FAIL_EXIT:
    set_eventfd(sige);
    pthread_join(signal_thread, NULL);
CREATE_THREAD_FAIL_EXIT:
    close(sige);
CREATE_EVENTFD_FAIL_EXIT:
    espress_stream_end_interface(ifd);
BEGIN_INTERFACE_FAIL_EXIT:
    espress_stream_end();
BEGIN_STREAM_FAIL_EXIT:
SCAN_ARG_FAIL_EXIT:

    return ret;
}
```

</details>

## サンプルアプリケーション

パッケージの<tt>arch/linux/app</tt>ディレクトリ以下にlinux用の各種サンプルアプリケーションが存在しています。
詳細は各<tt>README.md</tt>を参照ください。

<tt>es_ccat</tt>, <tt>es_scat</tt>
: 接続している2つのアプリケーションにおいて、一方の標準入力からの入力がもう一方の標準出力から出力されます。

<tt>es_ftx</tt>, <tt>es_frx</tt>
: クライアントアプリケーション(<tt>es_ftx</tt>)からサーバーアプリケーション(<tt>es_frx</tt>)にファイルを送信します。

<tt>es_gw</tt>
: 任意のespress_streamクライアントアプリケーションからの接続を受け付けたサーバーアプリケーションが、この接続の入出力を、子プロセスで起動させたコマンドの標準入出力に接続します。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2023 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
