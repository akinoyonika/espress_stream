# <tt>es_wg</tt>

## 概要

espress_streamクライアントアプリケーションと接続するespress_streamサーバアプリケーションで、クライアントからの接続を受け付けると指定されたアプリケーションを子プロセスで起動し、クライアントの入出力を子プロセスで起動したアプリケーションの標準入出力に接続します。

## 導入

### 準備

espress_streamライブラリーの導入環境を準備し、espress_streamライブラリーを導入しておいてください。

### 手順

```
$ cd espress_stream/arch/linux/app/es_gw
$ ./mk_cert_h.sh
$ make clean
$ make
$ sudo make uninstall
$ sudo make install
$ make clean
```

<tt>es_gw</tt>はサーバーアプリケーションです。
これに必要なサーバー認証情報は実行コードに埋め込まれます。
<tt>mk_cert_h.sh</tt>はこれらの情報を定義した<tt>es_scat</tt>用のソースコードの一部(<tt>cert.h</tt>)を生成します。

## 使用方法

<tt>es_gw</tt>の実行には標準入出力で何らかのデータを送受するアプリケーションを指定する必要があります。
ここでは条件に適合する最も単純なアプリケーションとして<tt>cat</tt>を使用することとします。
<tt>cat</tt>はオプションを何も指定しないと標準入力からの入力を標準出力にそのまま出力します。

```
$ es_gw cat
public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
protocol: ipv4
bind address: (any)
bind port: 12345
close: full
cat command: cat
```

<tt>public key</tt>はクライアントが行うサーバー認証のための公開鍵です。
ポート番号はデフォルトで<tt>12345</tt>になっていますので、変更したい場合は<tt>-p</tt>オプションを使用してください。

```
$ es_gw -p 12345 cat
public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
protocol: ipv4
bind address: (any)
bind port: 12345
close: full
cat command: cat
```

<tt>es_gw</tt>はespress_streamのサーバーアプリケーションですので、何かespress_streamクライアントアプリケーションを使ってes_gwに接続しなければ何も機能しません。
ここでは<tt>es_ccat</tt>を接続してみます。
<tt>es_ccat</tt>はespress_streamのlinux用サンプルアプリケーションの一つです。
<tt>es_ccat</tt>は接続したespress_streamサーバーに対し、標準入力をそのまま出力し、サーバーからの入力をそのまま標準出力に出力します。

```
$ es_ccat -a foo -p 12345 -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
destination address: foo
destination port: 12345
destination public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
close: full
```

<tt>-a foo</tt>は受信マシンのドメイン名指定です (<tt>###.###.###.###</tt>)形式でIPアドレス指定可)。
<tt>-k 7335...</tt>はサーバーの公開鍵です。サーバー実行時に表示されるものをそのまま指定してください。

接続に成功すると1行の"<tt>CONNECT</tt>"が出力され、それ以降、入力がエコーバックされるようになります。

入力を閉じると<tt>es_ccat</tt>が終了します。
標準入力がコンソールの場合、[CTRL] + [D]で入力を閉じることができます。
<tt>es_gw</tt>は[CTRL] + [C]の入力で終了します。

<tt>es_gw</tt>の本来の目的はespress_streamクライアントによる通信を任意のストリーム通信に変換することです。
たとえば、<tt>es_gw</tt>に対して<tt>nc</tt>コマンドを指定することでespress_streamプロトコルをTCP/IPプロトコルに変換できます。

```
$ es_gw -p 12345 nc google.com 80
public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
protocol: ipv4
bind address: (any)
bind port: 12345
close: full
cat command: nc google.com 80
```

<tt>es_ccat</tt>で<tt>es_gw</tt>に接続します。

```
$ es_ccat -a foo -p 12345 -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
destination address: foo
destination port: 12345
destination public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
close: full
```

1行の"<tt>CONNECT</tt>"が出力されたなら次のように入力します。

```
GET / HTTP/1.1

```

空行を忘れないようにしてください。
空行の後、google.comからのHTMLテキストが表示されます。

この時の動作は以下のようになります。

1. <tt>es_ccat</tt>の標準入力からのデータはespress_streamプロトコルを用い<tt>es_gw</tt>に伝えられます。
2. <tt>es_gw</tt>は子プロセスで立ち上げた<tt>nc</tt>の標準入力にこのデータを出力します。
3. <tt>nc</tt>の標準入力からのデータはTCP/IPプロトコルを用い<tt>google.com</tt>のウェッブサーバーに伝えられます。
4. <tt>google.com</tt>からの応答はTCP/IPプロトコルを用い<tt>nc</tt>に伝えられます。
5. <tt>nc</tt>は親プロセスである<tt>es_gw</tt>によってリダイレクトされている標準出力に応答を出力します。
6. <tt>es_gw</tt>の受け取った応答はespress_streamプロトコルを用いて<tt>es_ccat</tt>に伝えられます。
7. <tt>es_ccat</tt>は受け取った応答を標準出力に出力します。

## ユーセージ

### 注意

プロトコルにrawを指定する場合、実行にはroot権限もしくはCAP_NET_RAWケーパビリティが必要です。

### <tt>es_gw [-4 | -6] [-a \<bind address\>] [-p \<bind port\>] [-h] \<cat command\> [\<cat command param\> [...]]</tt>

<tt>-4</tt>
: プロトコルにipv4を使用する。 (デフォルト)

<tt>-6</tt>
: プロトコルにipv6を使用する。 (非デフォルト)

<tt>-a \<bind address\></tt>
: 結合するアドレス("<tt>nnn.nnn.nnn.nnn</tt>" or "<tt>xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx%ifname</tt>")を指定する。 (デフォルト: 任意)

<tt>-p \<bind port\></tt>
: 結合するポート番号を指定する。 (デフォルト値: "<tt>12345</tt>")

<tt>-h</tt>
: ハーフクローズ許可。 (デフォルト: ハーフクローズ禁止)

<tt>\<cat command\></tt>
: 標準入出力によるデータの入出力を行うコマンドを指定する。

<tt>\<cat command param\></tt>
: コマンドに対するオプションを指定する。

### <tt>es_gw -0 [-a \<interface name\>] [-p \<ether type\>] [-h] \<cat command\> [\<cat command param\> [...]]</tt>

<tt>-0</tt>
: プロトコルにrawを指定する。 (非デフォルト)

<tt>-a \<interface name\></tt>
: 使用するインターフェイスを指定する。 (デフォルト: 任意)

<tt>-p \<ether type\></tt>
: 使用するイーサタイプ番号を指定する。 (デフォルト: 任意)

<tt>-h</tt>
: ハーフクローズ許可。 (デフォルト: ハーフクローズ禁止)

<tt>\<cat command\></tt>
: 標準入出力によるデータの入出力を行うコマンドを指定する。

<tt>\<cat command param\></tt>
: コマンドに対するオプションを指定する。

## サーバー認証について

サーバー認証にはEd25519アルゴリズム[RFC 8032]を使用しています。
[OpenSSH](https://www.openssh.com/)の同アルゴリズム使用の認証方法に類似のものとなっています。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2025 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
