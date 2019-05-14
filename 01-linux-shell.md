# Linux Shell

## 1. Lý thuyết

### 1.1 Một vài khái niệm

#### 1.1.1. Dòng stream cơ bản

Có 3 dòng stream cơ bản trong linux:

- **stdin**: Thiết bị nhập input cho shell ví dụ như bàn phím
- **stdout**: Hiển thị kết quả các lệnh lên terminal cho ta thấy
- **stderr**: Hiển thị các lỗi trong quá trình thực thi một lệnh hay thực hiện một công việc nào đó.

Chuyển hướng output:
- Ta có thể sử dụng `>` để chuyển kết quả của một câu lệnh vào trong một file nào đó thay vì hiển thị ra màn hình
- Để ghi đè kết quả lên một file có sẵn ta dùng dấu `>>`

Chuyển hướng error sử dụng dấu `2>`

Đường ống dẫn (pipes - ký hiệu: `|`): Cho phép chúng ta lấy kết quả của lệnh trước dẫn đến cho lệnh phía sau nó.

#### 1.1.2. Bash Operators

- Command “chaining”

    - Sequence: `;` - Thực thi nhiều task. Sau khi task 1 hoàn thành thì đến task thứ 2, task thứ 2 hoàn thành đến task thứ 3...

    - Parallel execution: `&` - Thực thi các task cùng một lúc

- Conditional execution

    - And operator: `&&` - Task 2 chỉ được thực thi nếu task 1 thực thi thành công

    - Or operator: `||` - Task 2 chỉ được thực thi nếu task 1 thực thi không thành công

- Pipelines

    - Pipe operator: `|` - Được sử dụng để gửi output từ task 1 (stdout) đến input của task 2 (stdin)

    - Pipe error operator: `|&` - Error output của task 1 (stderr) cũng được chuyển tới input của task 2 (stdin)

- Input/output redirection

    - Redirecting input: `<` - Input đọc từ file (VD: `sort < list.txt`)

    - Redirecting input – here documents: `<<` - Được sử dụng khi input của lệnh nên được lấy trực tiếp từ terminal, thay vì từ file (VD: `sort << STOP`)

    - Redirecting input – here string:  `<<<` - Input đọc từ string (VD: `wc <<< "hello world"`)

    - Redirecting output – stdout: 
        - `>` - Ghi output vào file

        - `>>` - Ghi output vào file, ghi đè luôn file nếu nó tồn tại

    - Redirecting output – stderr: 
        - `2>` - Ghi stderr vào file
        - `2>>` - Ghi stderr vào file, ghi đè luôn file nếu nó tồn tại

    - Redirecting output – stdout and stderr: 
        - `&>` - Gửi stdout và stderr vào file
        - `&>>` - Gửi stdout và stderr vào file, ghi đè luôn file nếu nó tồn tại

### 1.2. grep
`grep` là 1 lệnh rất hữu ích trong việc tìm kiếm và khai thác nội dung văn bản bên trong tập tin.

#### 1.2.1 Tìm kiếm các dòng có chứa 1 mẫu cho trước
`grep <pattern> <filename>`

#### 1.2.2. Đọc dữ liệu từ stdin
VD: `echo -e "this is a world\nnext line" | grep word`

#### 1.2.3. Tìm kiếm cùng lúc nhiều tập tin
`grep <pattern> <file1> <file2> ...`

#### 1.2.4. Tô màu từ trùng khớp trong dòng
`grep <word> <filename> --color=auto`

#### 1.2.5. Sử dụng biểu thức chính quy
Thông thường, lệnh grep chỉ phiên dịch 1 vài ký tự đặc biệt bên trong `match_text`. Để sử dụng 1 tập đầy đủ các biểu thức chính quy như là đối số đầu vào, sử dụng tùy chọn `-E`, nghĩa là 1 biểu thức chính quy được mở rộng. Hoặc, chúng ta có thể sử dụng lệnh `egrep`.

VD:

`grep -E "[a-z]+" filename`

hoặc

`egrep "[a-z]+" filename`

#### 1.2.6. Chỉ xuất những phần văn bản trùng khớp
Để xuất ra chỉ những **phần văn bản** trùng khớp trong tập tin, thay cho việc phải xuất ra cả 1 dòng, sử dụng tùy chọn `-o`.

VD: 

```
$ echo this is a line. | egrep "[a-z]+\."
this is a line.
```

VD: 

```
$ echo this is a line. | egrep -o "[a-z]+\."
line.
```

#### 1.2.7. In ra các dòng không chứa chuỗi trùng khớp
`grep -v <pattern> <filename>`

#### 1.2.8. Đếm số _dòng_ chứa chuỗi trùng khớp
`grep -c <pattern> <filename>`

VD: 

```
$ echo -e "1 2 3 4\nhello\n5 6" | egrep -c "[0-9]"
2
```

Dù cho có 6 lần trùng khớp, tùy chọn này sẽ in ra 2, vì chỉ có 2 dòng trùng khớp. Nhiều lần trùng khớp trên 1 dòng được tính là 1.

#### 1.2.9. Đếm số _lần_ trùng khớp
VD: 

```
$ echo -e "1 2 3 4\nhello\n5 6" | egrep -o "[0-9]" | wc -l
6
```

#### 1.2.10. Tìm vị trí của chuỗi trùng khớp
Dùng tùy chọn `-n`

VD:

```
$ cat sample.txt
gnu is not unix
linux is fun
bash is art
$ grep linux -n
sample.txt:2:linux is fun
```

Vậy, từ `linux` ở dòng 2 của `sample.txt`.

#### 1.2.11. Tìm vị trí ký tự của chuỗi trùng khớp
Để in ra vị trí ký tự hoặc byte nơi có 1 mẫu được trùng khớp, sử dụng:

```
$ echo gnu is not unix | grep -b -o "not"
7:not
```

Vị trí ký tự cho 1 chuỗi trong 1 dòng được đềm từ 0, bắt đầu với ký tự đầu tiên.

#### 1.2.12. Liệt kê các tập tin chứa chuỗi trùng khớp
`grep -l <pattern> <file1> <file2> <file3> ...`

Ngược lại với đối số `-l` là `-L`. Đối số `-L` trả về 1 danh sách các tập tin không trùng khớp.

#### 1.2.13. Tìm kiếm đệ quy nhiều tập tin
Dùng tùy chọn `-R` hoặc `-r` đều được vì chúng có cùng ý nghĩa khi dùng với `grep`.

`grep <pattern> <path> -R` : Tìm pattern trong đường dẫn path (nếu dùng `.` là thư mục hiện tại) dùng đệ quy nhiều tập tin.

#### 1.2.14. Tìm kiếm không phân biệt chữ hoa, chữ thường
`grep -i <pattern> <filename>`

#### 1.2.15. Đối chiếu nhiều mẫu trong grep
Thông thường, chúng ta chỉ sử dụng 1 mẫu để đối chiếu khi sử dụng grep. Tuy nhiên, lệnh grep còn làm được nhiều hơn thế, chúng ta có thể đối chiếu cùng lúc nhiều mẫu bằng việc sử dụng đối số `-e`:

`grep -e <pattern1> -e <pattern2> ...`

Chúng ta có thể dùng một tập tin chứa các mẫu cần đối chiếu rồi dùng tùy chọn `-f`:

`grep -f <pattern_file> <source_file>`

#### 1.2.16. Thêm và loại bỏ các tập tin tìm kiếm

VD: Để tìm kiếm đệ quy chỉ các tập tin .c và .cpp trong 1 thư mục (thư mục hiện tại chẳng hạn) và bỏ qua các loại tập tin khác, ta sử dụng cú pháp sau:

`grep <pattern> . -r --include *.{c,cpp}`

VD: Để tìm kiếm tất cả tập tin, ngoại trừ các tập tin README, sử dụng cú pháp sau:

`grep <pattern> . -r --exclude "README"`

#### 1.2.17. Sử dụng grep với xargs

Lệnh xargs thường được dùng để cung cấp 1 danh sách các tên tập tin như 1 đối số dòng lệnh cho các lệnh khác. Khi các tên tập tin được sử dụng như các đối số dòng lệnh, chúng ta nên sử dụng ký tự `zero-byte (\0)` làm ký tự phân biệt các tên tập tin với nhau thay cho ký tự `khoảng trắng`.

Xem thêm về `xargs` ở các phần sau.

#### 1.2.18. Chế độ im lặng trong grep
Dùng tùy chọn `-q`, lúc này lệnh grep sẽ không in ra các chuỗi trùng khớp, thay vào đó nó sẽ trả về trạng thái là `thành công (0)` hay `thất bại (khác 0)`.

#### 1.2.19. In các dòng trước và sau các chuỗi văn bản trùng khớp
Thông thường grep sẽ in ra dòng chứa chuỗi văn bản trùng khớp. Nhưng chúng ta có thể cần “n” dòng sau dòng trùng  khớp, hoặc “n” dòng trước dòng trùng khớp, hoặc cả 2. Chúng ta thực hiện việc này việc sử dụng điều khiển dòng bối cảnh trong grep. Cụ thể:

`grep <pattern> -A <number> ...` : Tìm `number` dòng sau dòng trùng khớp.

`grep <pattern> -B <number> ...` : Tìm `number` dòng trước dòng trùng khớp.

`grep <pattern> -C <number> ...` : Tìm `number` dòng trước và sau dòng trùng khớp.

### 1.3. find

#### 1.3.1. Tìm theo tên
##### Sử dụng tên file để tìm
`find -name <filename>`

VD:

```
$ find -name "Test.sh"
/home/Test.sh
/home/yourname/Test.sh
```

##### Sử dụng tên nhưng sẽ không phân biệt hoa thường**
`find -iname >filename>`

VD:

```
$ find -iname "Test.sh"
/home/Test.sh
/home/yourname/Test.sh
/home/yourname/test.sh
/home/yourname/test
/home/yourname/test/TEST.sh
```

- **find sẽ tìm theo cả tên thư mục** nên để hạn chế, bạn hãy chỉ định loại cần tìm là chỉ file mà thôi bằng cách thêm vào `-type f`. Tìm thư mục sẽ là `-type d`:
VD: `find -name "Test.sh" -type f`

- Nếu bạn muốn tìm các file và thư mục không có tên là `Test.sh` thì hãy thêm `-not` vào trước `-name` hay `-iname` để phủ định kết quả.
VD: `find -not -name "Test.sh"`

#### 1.3.2. Tìm theo thư mục
Chỉ định thư mục tìm kiếm.

Ngoài ra, nếu bạn không muốn tìm quá sâu bên trong thư mục đó, bạn sẽ dùng đến `mindepth` và `maxdepth`.

VD: Tìm trong thư mục /home và trong thư mục con trực tiếp của nó

```
$ find /home -iname "Test.sh" -maxdepth 1 -type f
/home/Test.sh
/home/yourname/Test.sh
/home/yourname/test.sh
```

Hơn nữa bạn có thể sử dụng kết hợp với mindepth để tìm trong khoảng level nào đó của thư mục :
VD: 

```
$ find /home -iname "Test.sh" mindepth 1 -maxdepth 2 -type f
/home/yourname/Test.sh
/home/yourname/test.sh
/home/yourname/test/TEST.sh
```

#### 1.3.3 Thực thi lệnh với các file tìm được
Đôi khi chúng ta muốn kết hợp luôn các command khác với find, để xử lý các kết quả tìm được, như xóa chúng đi, hay copy đến thư mục chỉ định... lúc đó bạn cần dùng đến `-exec` (viết tắt của execute). 

VD: Như bên dưới tôi muốn tính toán mã md5 của các file tìm được :

```
$ find /home -type f -iname "Test.sh" -maxdepth 1 -exec md5sum {} \;
d4dd8cd98f00b204e9800998fjgwjwor /home/Test.sh
r31g8cd98f00b204e98009953j0f27g8 /home/yourname/Test.sh
9k9d8cggkf00b204e980fd9sgs8s69gk /home/yourname/test.
```

Các bạn thấy ở trên tôi dùng `{}` đây là đối số truyền vào cho md5sum, nó là file hiện tại khi find tìm thấy 1 file nào đó. Còn `;` là để kết thúc việc thực thi với file hiện tại đó.

VD: Chạy lệnh này tức là bạn đã remove hết các file tìm được bởi find.
`find /home -type f -iname "Test.sh" -maxdepth 1 -exec rm {} \;`

#### 1.3.4. Tìm file trống (0 byte)
`find /home -empty`

- Hãy thêm `-exec rm {} \;` để loại bỏ những file trống không cần thiết.

- Bạn có thể xóa mất các file hệ thống empty, đôi khi nó trống nhưng lại có ích để làm gì đó.
`find /home -empty -not -name ".*" -exec rm {} \;`

### 1.3.5. Tìm file theo dung lượng
VD: `find /home -iname "test.*" -size 10M -type f`

- Bạn chỉ cần dùng `-10M` hay `+10M` để tìm những file `nhỏ hơn` hoặc `lớn hơn` 10M -> `thêm - hay + trước con số dung lượng file`.

#### 1.3.6. Tìm file theo thời gian
- Thời gian truy cập (access time) : là thời điểm cuối cùng mà file đó được bạn hay ai đó truy cập vào
- Thời gian chỉnh sửa (modification time) : nếu ai đó truy cập vào file mà có chỉnh sửa nội dung nào đó thì chính là thời gian chỉnh sửa này, nó khác với access time nhé.
- Thời gian thay đổi : mỗi file sẽ có một inode number riêng của chúng -> đây chính là thời điểm cuối cùng mà inode của 1 file bị thay đổi.

VD: Tìm kiếm tất cả các file Excel mà đã được chỉnh sửa trong vòng 1 ngày trước.
`find /home/myname/Downloads -name *.xlsx -type f -mtime -1`

- Để tìm trong vòng bao nhiêu phút thì bạn dùng `-mmin`
`find /home/myname/Downloads -name *.xlsx -type f -mmin -60`

- `m` ở đây là `modification`. Nếu bạn muốn tìm the thời gian truy cập thì dùng `-atime` hoặc `-amin`.

#### 1.3.7. Lưu kết quả tìm được ra file để tổng hợp
VD: `find /home/myname/Downloads -name *.xlsx -type f > result.txt`

- Đôi khi find command không đọc được file nào đó (file ẩn chẳng hạn) hoặc có lỗi nào đó sẽ bắn ra cả nội dung lỗi nên không cần thiết để lưu chúng cùng với kết quả. Bạn chỉ cần thêm `2>/dev/null` như sau:

`find /home/myname/Downloads -name *.xlsx -type f 2>/dev/null > result.txt`

Đoạn lệnh trên sẽ catch tất cả các lỗi xảy ra nếu có và đẩy vào /dev/null. Còn kết quả tìm vẫn được lưu vào result.txt

### 1.4. wc

Lệnh `wc` trong Linux được dùng để đếm số dòng, số từ hay số byte của 1 tập tin và in kết quả này ra màn hình.

`wc [OPTION] … [FILE]…`

`wc` in ra 1 dòng kết quả cho mỗi tập tin được truyền vào nó như đối số, kết quả được in theo thứ tự: dòng, từ, ký tự, byte, chiều dài dòng lớn nhất. Mặc định, wc in ra 3 giá trị: `số dòng, từ, và byte`.

Nếu có nhiều hơn 1 tập tin, `wc` in ra dòng cuối cùng là giá trị tổng của các tập tin trên.

#### 1.4.1. In ra số dòng của tập tin
Sử dụng tùy chọn `-l` hay `--lines` của wc.

#### 1.4.2. In ra số ký tự của tập tin
Để in ra số ký tự của 1 tập tin, sử dụng tùy chọn `-m` hay `--chars`.

#### 1.4.3. In ra số từ của tập tin
Để in ra số từ của 1 tập tin, sử dụng tùy chọn `-w` hay `--words`.

#### 1.4.4. In ra số byte của tập tin
Để in ra số byte của 1 tập tin, sử dụng tùy chọn `-c` hay `--bytes`.

#### 1.4.5. In ra chiều dài dòng dài nhất
Để in ra chiều dài của dòng dài nhất trong 1 tập tin, sử dụng tùy chọn `-L` hay `--max-line-length`.

Nếu có nhiều hơn 1 tập tin, lệnh wc in ra giá trị lớn nhất của những dòng đó.

#### 1.4.6. Lấy danh sách đối số từ tập tin

VD:

```
$ ls
file_1 file_2 file_3
$ find . -type f -print0 > file_list
$ wc --files0-from=file_list
2 10 47 ./file_1
3 15 70 ./file_2
4 20 96 ./file_3
0 1 39 ./file_list
9 46 252 total
```

#### 1.4.7. Sử dụng kết hợp với find
VD:

```
$ ls
file_1 file_2 file_3 file_list
$ find . -type f | wc -l
4
```

### 1.5. Quản lý process
PID – Process ID. Mỗi Process có 5 ký tự số. Những số này có thể hết (hết số) và bắt đầu lại, nhưng tại bất kỳ thời điểm nào, không có hơn 1 PID trong hệ thống.
PPID – Process Parent ID. ID của process mà khởi động process này.

2 commands phổ biến nhất được dùng để xem processes là `top` và `ps`. Khác biệt là top được dùng để tương tác nhiều còn ps được dùng trong scripts, kết hợp với các lệnh bash khác hoặc tương tự.

#### 1.5.1 Lệnh top
top – command top là đơn giản và phổ biến nhất để hiển thị những process chiếm nhiều tài nguyên máy tính nhất.

Hiển thị processes liên quan đến một user, bạn có thể dùng lệnh sau: `top -u user`

#### 1.5.2. Lệnh ps
ps – Một command hữu ích khác để hiển thị processes trong Linux.

#### 1.5.3. Lệnh kill
`kill pid`
Nếu process quá khó để kill, hãy dùng use: `kill -9 pid`

### 1.6. netstat
Hiển thị các kết nối TCP đang hoạt động, các cổng mà tại đó máy tính đang nghe và đồng thời hiển thị các thống kê Ethernet, bảng định tuyến IP, thống kê IPv4 (cho giao thức IP, ICMP, TCP và UDP) và số liệu thống kê IPv6 (cho IPv6, ICMPv6, TCP qua IPv6 và UDP qua giao thức IPv6). Không sử dụng tham số, netstat hiển thị các kết nối TCP đang hoạt động.

### 1.7. xargs
`xargs` là 1 lệnh rất hữu dụng trong việc xử lý chuyển đổi dữ liệu từ stdin sang đối số dòng lệnh. Nó có thể thao tác stdin và chuyển đối nó thành các đối số dòng lệnh cho 1 lệnh được chỉ rõ. Ngoài ra, xargs có thể chuyển đổi các dữ liệu văn bản 1 hay nhiều dòng bất kỳ thành các định dạng khác, như là nhiều dòng hay 1 dòng và ngược lại.

xargs có thể hoạt động như 1 sự thay thế cho đối số -exec trong lệnh find.

#### 1.7.1. Chuyển đổi nhiều dòng dữ liệu nhập vào thành 1 dòng dữ liệu xuất ra
VD: 

```
$ cat example.txt
1 2 3 4 5 6       
7 8 9 10
11 12
```

Thay '\n' thành ' '
VD: 

```
$ cat example.txt | xargs
1 2 3 4 5 6 7 8 9 10 11 12
```

#### 1.7.2. Chuyển đổi 1 dòng thành nhiều dòng
Bằng việc cung cấp số lượng các đối số tối đa trong 1 dòng, chúng ta có thể chia bất kỳ văn bản nào từ stdin thành các dòng có n đối số. Mỗi đối số là 1 phần của 1 chuỗi được phân tách nhau bởi ký tự khoảng trắng ” ” (space). **Khoảng trắng là ký tự phân tách mặc định.** 1 dòng có thể được chia thành nhiều dòng như sau:

VD: Có 3 đối số tối đa trên 1 dòng, các đối số phân biệt bằng " "

```
cat example.txt | xargs -n 3
1 2 3           
4 5 6           
7 8 9           
10 11 12        
```

Chúng ta cũng có thể sử dụng dấu phân cách của riêng mình để phân biệt các đối số. Để chỉ định 1 dấu phân tách tùy chọn, sử dụng tùy chọn `-d` như sau:
VD: `echo "splitXsplitXsplitXsplit" | xargs -d X`
split split split split

Bằng việc sử dụng -n cùng với lệnh trước, chúng ta có thể chia dữ liệu nhập vào thành nhiều dòng như sau:
VD: `echo "splitXsplitXsplitXsplit" | xargs -d X -n 2`
split split
split split

Bằng việc sử dụng `-p`, để hỏi user có muốn thực thi không

#### 1.7.3. Truyền các đối số được định dạng đến 1 lệnh
- Cách 1: Cung cấp từng đối số cho lệnh

```
./cecho.sh arg1
./cecho.sh arg2
./cecho.sh arg3
```

hoặc

```
./cecho.sh arg1 arg2
./cecho.sh arg3
```

- Cách 2: Cung cấp tất cả đối số
`./cecho.sh arg1 arg2 arg3`

VD:
Ta có file script sau: In ra danh sách các đối số

```
#!/bin/bash
#Filename: cecho.sh
echo $*'#'
```

File args.txt dạng:
```
arg1
arg2
arg3
```

Trường hợp này ta cần xem file args.txt, ta dùng dữ liệu trong file này để làm đối số cho script cecho.sh


```
$ cat args.txt | xargs -n 1 ./cecho.sh
arg1#
arg2#
arg3#
```

```
$ cat args.txt | xargs -n 2 ./cecho.sh
arg1 arg2#
arg3#
```

- Sử dụng tùy chọn `-I` để chỉ rõ 1 chuỗi thay thế sẽ được thay thế trong khi `xargs` mở rộng. Khi `-I` được dùng với `xargs`, nó sẽ thực thi lần lượt cho từng đối số.

VD: `cat args.txt | xargs -I {} ./cecho.sh -p {} -l`

`-I {}` chỉ ra chuỗi thay thế. Với mỗi đối số được cung cấp cho lệnh, chuỗi {} sẽ được thay thế với các đối số được đọc qua stdin.
Khi được dùng với -I, lệnh được thực thi trong 1 vòng lặp. Khi có 3 đối số lệnh sẽ được thực thi 3 lần cùng với lệnh {}. Mỗi lần thực thi, {} được thay bằng các đối số lần lượt từng cái một.

#### 1.7.4. Sử dụng xargs với find
VD: Tìm tập tin .txt và xóa chúng
`find . -name "*.txt" -type f -print0 | xargs -0 rm -f`
`-0` (tức là: `-d '\0'`) biên dịch ký tự phân cách là `\0 (null)`.

Ta có 3 file: 't 1.txt', 'h 2.txt' và 'g.txt'
- Khi ta find theo *.txt sẽ được: In ra cách nhau bởi `\n`

```
./t 1.txt
./g.txt
./h 2.txt
```

Vấn đề là khi ta dùng xargs định dạng đối số để truyền vào lệnh rm. Nếu ta làm thông thường, nó sẽ phân cách theo dấu khoảng trắng
`find ... | xargs`

```
./t 1.txt ./g.txt ./h 2.txt         #nó sẽ hiểu là 5 đối số
```

- Khi ta find có thêm -print0 sẽ được: In ra cách nhau bởi `\0`

```
./t 1.txt./g.txt./h 2.txt
```

Ta không phân cách theo khoảng trắng nữa mà theo ký tự kết thúc chuỗi `\0`.
`find ... -print0 | xarg -0`


#### 1.7.5. Đếm số dòng mã C trong 1 thư mục
`find <source_code_dir_path> -type f -name "*.c" -print0 | xargs -0 wc -l`

#### 1.7.6. Sử dụng while và subshell với stdin
xargs bị giới hạn trong việc cung cấp các đối số theo những cách giới hạn. xargs không thể cung cấp các đối số đến nhiều tập hợp lệnh. Với các lệnh thực thi với 1 tập hợp các đối số từ stdin, chúng ta có 1 cách rất linh hoạt. 1 `subshell` (1 khối chương trình nhỏ được viết bằng Bash shell) với 1 vòng lặp while có thể được dùng để đọc các đối số và thực thi các lệnh 1 cách phức tạp hơn như sau:

`cat files.txt | ( while read arg; do cat $arg; done )`

Lệnh trên tương tự với:

`cat files.txt | xargs -I {} cat {}`

Bằng việc thay thế cat `$arg` với bất kỳ lệnh nào trong vòng lặp while, chúng ta có thể thực hiện nhiều lệnh với cùng đối số. Chúng ta cũng có thể truyền kết quả đến các lệnh khác mà không sử dụng pipe. Thủ thuật `subshell ( )` có thể được dùng trong nhiều môi trường khác nhau. Khi được bao đóng với các toán tử subshell, nó hoạt động như 1 đơn vị với nhiều dòng lệnh bên trong.

VD:
`cmd0 | ( cmd1;cmd2;cmd3) | cmd4`

Nếu `cmd1` là `cd /`, bên trong `subshell`, đường dẫn của thư mục làm việc sẽ thay đổi. Tuy nhiên, việc thay đổi này chỉ xảy ra bên trong subshell. `cmd4` sẽ không thấy được sự thay đổi trên.

### 1.8. awk
wk là 1 công cụ được thiết kế để làm việc với các dòng dữ liệu. Nó có thể làm việc trên nhiều cột và nhiều dòng của dòng dữ liệu. Nó hỗ trợ nhiều chức năng có sẵn, như là mảng và hàm, trong ngôn ngữ lập trình C. Lợi thế lớn nhất của nó là tính linh hoạt của nó.

#### 1.8.1. Cấu trúc
VD: `awk 'BEGIN{ print "start" } pattern { commands } END{ print "end" } file`

1 kịch bản awk thường bao gồm 3 phần:

- `BEGIN{ commands }` => chứa các khai báo được thực thi trước khi awk đọc nội dung dữ liệu
- `pattern { commands }` => gồm có pattern (các điều kiện) dùng để lọc nội dung các dòng dữ liệu và { commands } là các khai báo sẽ được thực thi trên các dòng trùng khớp với pattern.
- `END{ commands }` => chứa các khai báo được thực thi sau khi awk đọc xong nội dung dữ liệu.

Nội dung của kịch bản awk thường được bao đóng trong cặp dấu ngoặc đơn '' hoặc ngoặc kép "":

Lệnh awk hoạt động theo cách sau:

- Thực thi các khai báo trong khối `BEGIN { commands }`.
- Đọc 1 dòng từ tập tin hoặc stdin, và thực thi khối `pattern { commands }`. Lặp lại bước này cho đến cuối tập tin.
- Khi đến cuối dòng dữ liệu nhập vào, thực thi khối `END { commands }`.

Khối quan trọng nhất là khối `pattern { commands }`, khối này gồm 2 thành phần:

- `pattern` (mẫu): là mẫu đối chiếu được cung cấp, dùng để đối chiếu, so sánh với nội dung của các dòng dữ liệu. Pattern có thể là 1 biểu thức chính quy, điều kiện, 1 dãy các dòng .v.v…
- `{ commands }`: là các lệnh sẽ được thực thi với các dòng dữ liệu trùng khớp với pattern ở trên.

#### 1.8.2. Các biến đặc biệt
- NR: Số của bản ghi hiện tại, tương ứng với số dòng hiện tại khi nó sử dụng các dòng như các bản ghi.
- NF: Số lượng các trường (cột), tương ứng với số lượng các trường trong bản ghi hiện tại đang thực thi (các trường được phân cách bởi khoảng trắng).
- $0: Biến chứa nội dung văn bản của dòng hiện tại đang thực thi
- $1: Biến chứa văn bản của trường thứ nhất.
- $2: Biến chứa văn bản của trường thứ hai.
    ...
- $n: Biến chứa văn bản của trường thứ n (với n != 0)

VD:

```
$ echo -e "line1 f2 f3\nline2 f4 f5\nline3 f6 f7" | awk '{ print "Line no:"NR",No of fields:"NF, "$0="$0, "$1="$1,"$2="$2,"$3="$3 }'
Line no:1,No of fields:3 $0=line1 f2 f3 $1=line1 $2=f2 $3=f3
Line no:2,No of fields:3 $0=line2 f4 f5 $1=line2 $2=f4 $3=f5
Line no:3,No of fields:3 $0=line3 f6 f7 $1=line3 $2=f6 $3=f7
```

VD: Tính tổng tất cả các số trong mỗi dòng của trường thứ 1:

```
$ seq 5 | awk 'BEGIN{ sum=0; print "Summation:" } { print $1"+"; sum+=$1 } END{ print "=="; print sum }'
Summation:
1+
2+
3+
4+
5+
==
15
```

#### 1.8.3. Truyền biến bên ngoài vào awk
VD:

```
$ VAR=10000
$ echo | awk -v VARIABLE=$VAR '{ print VARIABLE }'
10000
```

```
$ var1="Variable1"; var2="Variable2"
$ echo | awk '{ print v1,v2 }' v1=$var1 v2=$var2
Variable1 Variable2
```

#### 1.8.4. Đọc 1 dòng tường minh sử dụng getline
Thông thường, awk đọc tất cả các dòng trong 1 tập tin. Nếu chúng ta muốn đọc 1 dòng cụ thể, chúng ta có thể sử dụng hàm getline.

Cú pháp là `getline var`. Biến `var` sẽ chứa nội dung của dòng. Nếu getline được gọi không có đối số, chúng ta có thể truy cập nội dung của dòng bằng việc sử dụng các biến $0, $1 và $2.

```
$ seq 5 | awk 'BEGIN { getline; print "Read ahead first line", $0 } { print $0 }'
Read ahead first line 1
2
3
4
5
```

#### 1.8.5. Lọc các dòng được xử lý bởi awk với các mẫu lọc (pattern)
Để làm việc với 4 dòng đầu:

```
$ seq 5 | awk 'BEGIN{ print "Start" }NR<5 { print } END{ print "Finish" }'
Start
1
2
3
4
Finish
```

hoặc

```
$ seq 5 | awk 'BEGIN{ print "Start" } NR==1,NR==4 { print } END{ print "Finish" }'
Start
1
2
3
4
Finish
```

Xử lý những dòng có chứa chuỗi "linux"

```
$ cat data.txt
linux
debian
unix
windows
```

```
$ cat data.txt | awk 'BEGIN { print "Start" } /linux/ { print } END{ print "Finish" }'
Start
linux
Finish
```

Xử lý những dòng không chứa chuỗi “linux”

```
$ awk 'BEGIN { print "Start" } !/linux/ { print } END{ print "Finish" }' data.txt
```

hoặc

```
$ cat data.txt | awk 'BEGIN { print "Start" } !/linux/ { print } END{ print "Finish" }'
Start
debian
unix
windows
```

#### 1.8.6. Thiết lập dấu phân cách cho các trường
Dấu phân cách mặc định cho các trường trong awk là `khoảng trắng`. Chúng ta có thể chỉ định 1 cách tường minh 1 dấu phân cách khác bằng việc sử dụng `-F “delimiter”`.

```
$ awk -F: '{ print $NF }' /etc/passwd
```

hoặc

```
$ awk 'BEGIN { FS=":" } { print $NF }' /etc/passwd
```

Chúng ta có thể thiết lập dấu phân cách các trường kết quả xuất ra bằng việc thiết lập `OFS=”delimiter”` trong khối BEGIN.

Ví dụ:

```
$ awk 'BEGIN { FS=":" } { print $1,$2 }' /etc/passwd
root x
bin x
daemon x
adm x
....
```

```
$ awk 'BEGIN { FS=":"; OFS=":" } { print $1,$2 }' /etc/passwd
root:x
bin:x
daemon:x
adm:x
...
```

#### 1.8.7. Sử dụng vòng lặp trong awk

```
for(i=0;i<10;i++) { print $i; }
```

hoặc

```
for (i in array) { print array[i]; }
```

#### 1.8.8. Các hàm thao tác chuỗi trong awk
- `length(string)`: trả về chiều dài của chuỗi.
- `index(string, search_string)`: Trả về vị trí mà search_string được tìm thấy trong string.
- `split(string,array,delimiter)`: lưu trữ 1 danh sách các chuỗi được tạo bằng việc sử dụng dấu phân cách trong mảng.
- `substr(string, start-position, end-position)`: Trả về chuỗi con được tạo từ chuỗi có nội dung từ vị trí bắt đầu (start-position) đến vị trị kết thức (end-position).
- `sub(regex, replacement_str, string)`: Thay thế chuỗi đầu tiên trùng khớp với biểu thức chính quy (regex) bằng chuỗi thay thế (replacement_str).
- `gsub(regex, replacement_str, string)`: Tương tự với hàm sub(), nhưng nó thay thế mọi trường hợp trùng khớp với biểu thức chính quy (regex).
- `match(regex, string)`: Trả về kết quả là 1 chuỗi trùng khớp với biểu thức chính quy (regex) có được tìm thấy trong string hay không. Nó trả về kết quả khác 0 nếu có trùng khớp, ngược lại thì trả về 0. 2 biến đặc biệt tương ứng với hàm match() là RSTART và RLENGTH. Biến RSTART chứa vị trí nơi chuỗi trùng khớp bắt đầu. Biến RLENGTH chứa độ dài của chuỗi trùng khớp với biểu thức chính quy.

### 1.9. sed
ed tượng trưng cho stream editor. Nó là 1 công cụ rất hữu ích cho việc xử lý văn bản, và là 1 tiện ích tuyệt vời để làm việc với các biểu thức chính quy. Việc sử dụng phổ biến nhất của sed là thay thế văn bản.

#### 1.9.1. Cú pháp
sed có thể được dùng để thay thế những xuất hiện của 1 chuỗi với 1 chuỗi khác trong 1 văn bản.

```
$ sed 's/pattern/replace_string/' file
```

hoặc

```
$ cat file | sed 's/pattern/replace_string/'
```

Trong đó, `pattern` có thể là chuỗi ký tự hoặc 1 biểu thức chính quy. Trong 1 văn bản, chuỗi cần thay thế (pattern) có thể xuất hiện từ 0 đến nhiều lần, mỗi lần như vậy được gọi là 1 xuất hiện của chuỗi cần thay thế.

#### 1.9.2. Lưu các thay đổi vào tập tin
Mặc định, sed chỉ in ra các văn bản được thay thế. Để lưu các thay đổi này vào cùng 1 tập tin, sử dụng tùy chọn `-i`.

```
$ sed 's/text/replace/' file > newfile
$ mv newfile file
```

hoặc

```
$ sed -i 's/text/replace/' file
```

#### 1.9.3 Thay thế tất cả xuất hiện của mẫu
Nếu chúng ta sử dụng các cú pháp đã đề cập ở trên, sed sẽ chỉ thay thế sự xuất hiện **đầu tiên** của mẫu (pattern) trong **mỗi dòng**. Nếu chúng ta muốn thay thế tất cả xuất hiện của mẫu trong văn bản, chúng ta cần thêm tham số `g` vào cuối như sau:

```
$ sed 's/pattern/replace_string/g' file
```

Tuy nhiên, đôi khi chúng ta không muốn thay thế tất cả xuất hiện của mẫu, mà chỉ muốn thay thế từ xuất hiện thứ N của mẫu đến cuối văn bản. Để làm việc này, chúng ta có thể sử dụng dạng `/Ng` như sau:

Ví dụ:

```
$ echo thisthisthisthis | sed 's/this/THIS/2g'
thisTHISTHISTHIS
```

Nếu chúng ta chỉ muốn thay thế đúng cái xuất hiện thứ N của mẫu trong văn bản, sử dụng dạng `/N` như sau:

```
$ echo thisthisthisthis | sed 's/this/THIS/2'
thisTHISthisthis
```

#### 1.9.4 Ký tự dấu phân cách
Trong các ví dụ trên, chúng ta đã sử dụng / làm ký tự dấu phân cách của sed. Chúng ta có thể sử dụng các ký tự phân cách bất kỳ như sau:

```
$ sed 's:text:replace:g'
$ sed 's|text|replace|g'
```

Khi 1 ký tự phân cách xuất hiện bên trong mẫu, chúng ta phải thoát nó bằng việc sử dụng tiền tố `\`, như sau:

```
$ sed 's|te\|xt|replace|g'
```

#### 1.9.5. Xóa các dòng trống

```
$ sed '/^$/d' file
```

`/pattern/d` sẽ xóa các dòng trùng khớp với `pattern`.

#### 1.9.6. Xóa các dòng chứa 1 chuỗi bất kỳ
VD: Xóa các dòng chứa chuỗi “mobile phones” trong tập tin sentence.txt

```
$ sed 's/ [^.]*mobile phones[^.]*\.//g' sentence.txt
```

#### 1.9.7. Ký hiệu chuỗi trùng khớp (&)
Trong sed, chúng ta có thể sử dụng & như 1 chuỗi trùng khớp cho các mẫu thay thế, theo cách này, chúng ta có thể sử dụng chuỗi trùng khớp này trong chuỗi thay thế.

```
$ echo this is an example | sed 's/\w\+/[&]/g'
[this] [is] [an] [example]
```

Ở đây, biểu thức chính quy `\w\+` đối chiếu tất cả các từ trong văn bản. Sau đó, chúng ta thay thế nó với [&], `&` tương ứng với từ trùng khớp.

#### 1.9.8. Ký hiệu đối chiếu chuỗi con (\1)
VD:

```
$ echo this is digit 7 in a number | sed 's/digit \([0-9]\)/\1/'
this is 7 in a number
```

Lệnh trên thay thế `digit 7` với `7`. Chuỗi con trùng khớp là `7`. `\(pattern\)` được dùng để đối chiếu chuỗi con. Mẫu này được bao đóng trong `()`, và được thoát với `\`. Với chuỗi con đầu tiên trùng khớp, ký hiệu tương ứng là `\1`; với chuỗi con thứ 2, ký hiệu là `\2`, và cứ thế.

VD:

```
$ echo seven EIGHT | sed 's/\([a-z]\+\) \([A-Z]\+\)/\2 \1/'
EIGHT seven
```

\([a-z]\+\) khớp với từ đầu tiên, và \([A-Z]\+\) khớp với từ thứ 2. \1 và \2 được dùng cho việc tham chiếu đến chúng. Trong phần thay thế, thứ tự của chúng được đổi thành \2 \1 và, do vậy, nó xuất hiện với thứ tự ngược lại.

#### 1.9.9. Kết hợp nhiều biểu thức
Ví dụ:

```
$ echo abc | sed 's/a/A/' | sed 's/c/C/'
AbC
$ echo abc | sed 's/a/A/;s/c/C/'
AbC
$ echo abc | sed -e 's/a/A/' -e 's/c/C/'
AbC
```

## 2. Bài tập

### 2.1. Processing texts

#### 2.1.1. Count the number of lines satisfying a specific pattern in a log file

Đếm số lượng dòng thỏa mãn một pattern cụ thể trong log file.

```
#!/bin/bash
grep -c $1 $2
```

Cách chạy:
```
$ sudo chmod +x <script>
$ ./<script> <pattern> <filename>
```

Ví dụ: `./countNumberOfLines.sh "ok" test.log`

#### 2.1.2. Calculate KLOC of code C/C++ files in a directory

Tính KLOC của các file code C/C++ trong một thư mục.



### 2.2. System


### 2.3. Shell Scripting


## 3. Nguồn tham khảo

- [10 Useful Chaining Operators in Linux with Practical Examples](https://www.tecmint.com/chaining-operators-in-linux-with-practical-examples/)

- [Bash Operators in Linux](https://www.luxoft-training.com/news/bash-operators-in-linux/)

- [Các lệnh trong Linux](http://www.justpassion.net/tech/programming/bash-shell)

- [find in Linux](https://viblo.asia/p/cung-tim-hieu-find-command-trong-linux-ymwGXVPnR4p1)

- [Bash Operators in Linux](https://www.luxoft-training.com/news/bash-operators-in-linux/)





