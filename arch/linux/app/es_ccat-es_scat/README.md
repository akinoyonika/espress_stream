# <tt>es_ccat</tt>, <tt>es_scat</tt>

## 概要

接続している2つのアプリケーションにおいて、一方の標準入力からの入力がもう一方の標準出力から出力されます。

## 導入


### 準備

espress_streamライブラリーの導入環境を準備し、espress_streamライブラリーを導入しておいてください。

### 手順

```
$ cd espress_stream/arch/linux/app/es_ccat-es_scat/es_ccat
$ make clean
$ make
$ sudo make uninstall
$ sudo make install
$ make clean
$ cd ../es_scat
$ ./mk_cert_h.sh
$ make clean
$ make
$ sudo make uninstall
$ sudo make install
$ make clean
```

<tt>es_scat</tt>はサーバーアプリケーションです。
これに必要なサーバー認証情報は実行コードに埋め込まれます。
<tt>mk_cert_h.sh</tt>はこれらの情報を定義した<tt>es_scat</tt>用のソースコードの一部(<tt>cert.h</tt>)を生成します。

## 使用方法

まず、<tt>es_scat</tt>を立ち上げます。

```
$ es_scat
public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
bind address: (any)
bind port: 12345
close: full
```

<tt>public key</tt>はクライアントが行うサーバー認証のための公開鍵です。
ポート番号はデフォルトで<tt>12345</tt>になっていますので、変更したい場合は<tt>-p</tt>オプションを使用してください。

```
$ es_scat -p 12345
public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
bind address: (any)
bind port: 12345
close: full
```

なお、接続待ちで30秒がすぎると<tt>es_scat</tt>は自動的に終了します。

次に、<tt>es_ccat</tt>を立ち上げます。

```
$ es_ccat -a foo -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
destination address: foo
destination port: 12345
destination public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
close: full
```

<tt>-a foo</tt>は受信マシンのドメイン名指定です (<tt>###.###.###.###</tt>)形式でIPアドレス指定可)。
<tt>-k 7335...</tt>はサーバーの公開鍵です。サーバー実行時に表示されるものをそのまま指定してください。

<tt>es_scat</tt>と同様、ポート番号はデフォルトで<tt>12345</tt>になっていますので、変更したい場合は<tt>-p</tt>オプションを使用してください。

```
$ es_ccat -a foo -p 12345 -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
destination address: foo
destination port: 12345
destination public key: 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9
close: full
```

接続に成功すると双方に1行の"<tt>CONNECT</tt>"が出力され、それ以降、一方の入力がもう一方に出力されるようになります。

どちらかの入力を閉じると双方のアプリケーションが終了します。
標準入力がコンソールの場合、[CTRL] + [D]で入力を閉じることができます。

## 標準入出力のリダイレクト

以下のようにすることで、簡易的なファイル転送ができます。

```
$ es_scat -s > boo.txt
```

```
$ es_ccat -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9 < boo.txt
```

ここで<tt>-s</tt>オプションは受信データ以外の情報出力を抑制するためのものです。

双方向に行うことも可能です。

```
$ es_scat -s -h < boo.doc > boo.txt
```

```
$ es_ccat -s -h -k 7335e63a762123a941a25e5e538dd4d2e47d869280f83af7810e0b3dae8069c9 < boo.txt > boo.doc
```

ここで<tt>-h</tt>オプションはハーフクローズを使用するためのものです。
通常これらのアプリケーションは受信チャンネルが相手側から閉じられると、自身の送信チャンネルを閉じ、アプリケーションを終了します。
この場合、上記のような使い方をすると、先に送信を完了した側が送信チャンネルを閉じることによって、相手側も送信チャンネルを閉じてしまいます。
これにより送信データの欠落が発生することがあります。
<tt>-h</tt>オプションにより、受信チャンネルが相手側から閉じられることによる送信チャンネルの終了が抑制され、ハーフクローズ状態でデータ送信を維持できるようになります。

## ユーセージ

### 注意

プロトコルにrawを指定する場合、実行にはroot権限もしくはCAP_NET_RAWケーパビリティが必要です。

### <tt>es_ccat [-4 | -6] [-a \<destination address\>] [-p \<destination port\>] [-b \<bind address\>] [-q \<bind port\>] [-h] [-s]</tt>

<tt>-4</tt>
: プロトコルにipv4を使用する。 (デフォルト)

<tt>-6</tt>
: プロトコルにipv6を使用する。 (非デフォルト)

<tt>-a \<destination address\></tt>
: 接続先のドメイン名、アドレス("<tt>nnn.nnn.nnn.nnn</tt>" or "<tt>xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx</tt>")を指定する。 (デフォルト値: "<tt>localhost</tt>")

<tt>-p \<destination port\></tt>
: 接続先のポート番号を指定する。 (デフォルト値: "<tt>12345</tt>")

<tt>-b \<bind address\></tt>
: 結合するアドレス("<tt>nnn.nnn.nnn.nnn</tt>" or "<tt>xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx</tt>")を指定する。 (デフォルト: 任意)

<tt>-q \<bind port\></tt>
: 結合するポート番号を指定する。 (デフォルト: 任意)

<tt>-h</tt>
: ハーフクローズ許可。 (デフォルト: ハーフクローズ禁止)

<tt>-s</tt>
: サイレントモード許可。 (デフォルト: サイレントモード禁止)

### <tt>es_ccat -0 [-a \<destination address\>] [-p \<ether type\>] [-b \<interface name\>] [-h] [-s]</tt>

<tt>-0</tt>
: プロトコルにrawを指定する。 (非デフォルト)

<tt>-a \<destination address\></tt>
: 接続先のアドレス("<tt>xx:xx:xx:xx:xx:xx</tt>")を指定する。 (デフォルト値: "<tt>ff:ff:ff:ff:ff:ff</tt>")

<tt>-p \<ether type\></tt>
: 使用するイーサタイプ番号を指定する。 (デフォルト値: "<tt>0xffff</tt>")

<tt>-b \<interface name\></tt>
: 使用するインターフェイスを指定する。 (デフォルト値: "<tt>eth0</tt>")

<tt>-h</tt>
: ハーフクローズ許可。 (デフォルト: ハーフクローズ禁止)

<tt>-s</tt>
: サイレントモード許可。 (デフォルト: サイレントモード禁止)

### <tt>es_scat [-4 | -6] [-a \<bind address\>] [-p \<bind port\>] [-h] [-s]</tt>

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

<tt>-s</tt>
: サイレントモード許可。 (デフォルト: サイレントモード禁止)

### <tt>es_scat -0 [-a \<interface name\>] [-p \<ether type\>] [-h] [-s]</tt>

<tt>-0</tt>
: プロトコルにrawを指定する。 (非デフォルト)

<tt>-a \<interface name\></tt>
: 使用するインターフェイスを指定する。 (デフォルト: 任意)

<tt>-p \<ether type\></tt>
: 使用するイーサタイプ番号を指定する。 (デフォルト: 任意)

<tt>-h</tt>
: ハーフクローズ許可。 (デフォルト: ハーフクローズ禁止)

<tt>-s</tt>
: サイレントモード許可。 (デフォルト: サイレントモード禁止)

## サーバー認証について

サーバー認証にはEd25519アルゴリズム[RFC 8032]を使用しています。
[OpenSSH](https://www.openssh.com/)の同アルゴリズム使用の認証方法に類似のものとなっています。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2024 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
