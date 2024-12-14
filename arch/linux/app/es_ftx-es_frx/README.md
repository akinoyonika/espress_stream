# <tt>es_ftx</tt>, <tt>es_frx</tt>

## 概要

クライアントアプリケーション(<tt>es_ftx</tt>)からサーバーアプリケーション(<tt>es_frx</tt>)にファイルを送信します。

## 導入

### 準備

espress_streamライブラリーの導入環境を準備し、espress_streamライブラリーを導入しておいてください。

### 手順

```
$ cd espress_stream/arch/linux/app/es_ftx-es_frx/es_ftx
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

<tt>es_frx</tt>はサーバーアプリケーションです。
これに必要なサーバー認証情報およびクライアント認証情報は実行コードに埋め込まれます。
<tt>mk_cert_h.sh</tt>はこれらの情報を定義した<tt>es_frx</tt>用のソースコードの一部(<tt>cert.h</tt>)を生成します。
<tt>mk_cert_h.sh</tt>を実行するとユーザーIDとパスワードの入力を求められるので、入力してください。
ユーザー情報は複数設定できるので、繰り返し入力してください。
空行の入力で終了します。

## 使用方法

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
<tt>-a foo</tt>は受信マシンのドメイン名指定です (<tt>###.###.###.###</tt>)形式でIPアドレス指定可)。
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

## ユーセージ

### 注意

プロトコルにrawを指定する場合、実行にはroot権限もしくはCAP_NET_RAWケーパビリティが必要です。

### <tt>es_ftx [-4 | -6] [-a \<destination address\>] [-p \<destination port\>] [-b \<bind address\>] [-q \<bind port\>] [-k \<destination public key\>] [-i \<user id\>] [-w \<user password\>] \<file name\> [\<file name\> [...]]</tt>

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

<tt>-k \<destination public key\></tt>
: 接続先の公開鍵番号を指定する。 (デフォルト: 認証無)

<tt>-i \<user id\></tt>
: ユーザーIDを指定する。 (デフォルト値: "")

<tt>-w \<user password\></tt>
: ユーザーパスワードを指定する。 (デフォルト値: "")

<tt>\<file name\></tt>
: 送信するファイルの名前を指定する。

### <tt>es_ftx -0 [-a \<destination address\>] [-p \<ether type\>] [-b \<interface name\>] [-k \<destination public key\>] [-i \<user id\>] [-w \<user password\>] \<file name\> [\<file name\> [...]]</tt>

<tt>-0</tt>
: プロトコルにrawを指定する。 (非デフォルト)

<tt>-a \<destination address\></tt>
: 接続先のアドレス("<tt>xx:xx:xx:xx:xx:xx</tt>")を指定する。 (デフォルト値: "<tt>ff:ff:ff:ff:ff:ff</tt>")

<tt>-p \<ether type\></tt>
: 使用するイーサタイプ番号を指定する。 (デフォルト値: "<tt>0xffff</tt>")

<tt>-b \<interface name\></tt>
: 使用するインターフェイスを指定する。 (デフォルト値: "<tt>eth0</tt>")

<tt>-k \<destination public key\></tt>
: 接続先の公開鍵番号を指定する。 (デフォルト: 認証無)

<tt>-i \<user id\></tt>
: ユーザーIDを指定する。 (デフォルト値: "")

<tt>-w \<user password\></tt>
: ユーザーパスワードを指定する。 (デフォルト値: "")

<tt>\<file name\></tt>
: 送信するファイルの名前を指定する。

### <tt>es_frx [-4 | -6] [-a \<bind address\>] [-p \<bind port\>] [-o] \<dir name\></tt>

<tt>-4</tt>
: プロトコルにipv4を使用する。 (デフォルト)

<tt>-6</tt>
: プロトコルにipv6を使用する。 (非デフォルト)

<tt>-a \<bind address\></tt>
: 結合するアドレス("<tt>nnn.nnn.nnn.nnn</tt>" or "<tt>xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx%ifname</tt>")を指定する。 (デフォルト: 任意)

<tt>-p \<bind port\></tt>
: 結合するポート番号を指定する。 (デフォルト値: "<tt>12345</tt>")

<tt>-o</tt>
: ファイルの上書きを許可する。 (デフォルト: 上書き禁止)

<tt>\<dir name\></tt>
: ファイルを保存するディレクトリを指定する。

### <tt>es_frx -0 [-a \<interface name\>] [-p \<ether type\>] [-o] \<dir name\></tt>

<tt>-0</tt>
: プロトコルにrawを指定する。 (非デフォルト)

<tt>-a \<interface name\></tt>
: 使用するインターフェイスを指定する。 (デフォルト: 任意)

<tt>-p \<ether type\></tt>
: 使用するイーサタイプ番号を指定する。 (デフォルト: 任意)

<tt>-o</tt>
: ファイルの上書きを許可する。 (デフォルト: 上書き禁止)

<tt>\<dir name\></tt>
: ファイルを保存するディレクトリを指定する。

## サーバー認証について

サーバー認証にはEd25519アルゴリズム[RFC 8032]を使用しています。
[OpenSSH](https://www.openssh.com/)の同アルゴリズム使用の認証方法に類似のものとなっています。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2023 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
