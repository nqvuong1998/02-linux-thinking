- [1. Lý thuyết](#1-lý-thuyết)
    - [1.1. Containerlization là gì?](#11-containerlization-là-gì)
    - [1.2. Container là gì?](#12-container-là-gì)
    - [1.3. Cài đặt docker](#13-cài-đặt-docker)
    - [1.4. Cách hoạt động của docker](#14-cách-hoạt-động-của-docker)
    - [1.5. Các câu lệnh trong docker](#15-các-câu-lệnh-trong-docker)
    - [1.6. Dockerfile là gì?](#16-dockerfile-là-gì)
    - [1.7. Cách viết Dockerfile](#17-cách-viết-dockerfile)
        - [1.7.1. Thiết lập image gốc](#171-thiết-lập-image-gốc)
        - [1.7.2. Cài đặt ứng dụng](#172-cài-đặt-ứng-dụng)
        - [1.7.3. Cấu hình](#173-cấu-hình)
    - [1.8. Cách sử dụng Dockerfile](#18-cách-sử-dụng-dockerfile)
        - [1.8.1. Build docker image từ Dockerfile](#181-build-docker-image-từ-dockerfile)
        - [1.8.2. Tạo container từ image](#182-tạo-container-từ-image)
- [2. Bài tập](#2-bài-tập)
    - [2.1. Viết Dockerfile](#21-viết-dockerfile)
    - [2.2. Tạo image từ Dockerfile](#22-tạo-image-từ-dockerfile)
- [3. Nguồn tham khảo](#3-nguồn-tham-khảo)

# Docker

## 1. Lý thuyết

### 1.1. Containerlization là gì?

**Công nghệ ảo hóa vitualization**

![](https://viblo.asia/uploads/f83e4a3a-bc95-4a4d-af37-dbaa9e03d28f.png)

Bạn có thể đã nghe tới cái tên Vitualbox hay VMware rồi đúng không, đó đó chính nó đó. Với công nghệ này, trên một máy chủ vật lý mình có thể tạo được nhiều OS, tận dụng tài nguyên đã tốt hơn nhưng lại nảy sinh vấn đề tiếp.

Về tài nguyên: Khi bạn chạy máy ảo, bạn phải cung cấp "cứng" dung lượng ổ cứng cũng như ram cho máy ảo đó, bật máy ảo lên để đó không làm gì thì máy thật cũng phải phân phát tài nguyên.

**Công nghệ containerlization**

![](https://viblo.asia/uploads/5fdfbb3b-de87-4b24-a65c-3cf8753bfa15.png)

Với công nghệ containerlization, trên một máy chủ vật lý, ta sẽ sinh ra được nhiều máy con (giống với công nghệ ảo hóa vitualization), nhưng tốt hơn ở chỗ là các máy con này (Guess OS) đều dùng chung phần nhân của máy mẹ (Host OS) và chia sẻ với nhau tài nguyên máy mẹ.

Có thể nói là khi nào cần tài nguyên thì được cấp, cần bao nhiêu thì cấp bấy nhiêu, như vậy việc tận dụng tài nguyên đã tối ưu hơn. 

### 1.2. Container là gì?

Các phần mềm, chương trình sẽ được Container Engine ( là một công cụ ảo hóa tinh gọn được cài đặt trên host OS) đóng gói thành các container.

Thế Container là gì, nó là một giải pháp để chuyển giao phần mềm một cách đáng tin cậy giữa các môi trường máy tính khác nhau bằng cách:

- Tạo ra một môi trường chứa mọi thứ mà phần mềm cần để có thể chạy được.
- Không bị các yếu tố liên quan đến môi trường hệ thống làm ảnh hưởng tới.
- Cũng như không làm ảnh hưởng tới các phần còn lại của hệ thống.

Bạn có thể hiểu là ruby, rails, mysql ... kia được bỏ gọn vào một hoặc nhiều cái thùng (container), ứng dụng của bạn chạy trong những chiếc thùng đó, đã có sẵn mọi thứ cần thiết để hoạt động, không bị ảnh hưởng từ bên ngoài và cũng không gây ảnh hưởng ra ngoài.

Các tiến trình (process) trong một container bị cô lập với các tiến trình của các container khác trong cùng hệ thống tuy nhiên tất cả các container này đều chia sẻ kernel của host OS (dùng chung host OS).

Đây một nền tảng mở dành cho các lập trình viên, quản trị hệ thống dùng để xây dựng, chuyển giao và chạy các ứng dụng dễ dàng hơn. Ví dụ, bạn có một app java, bạn sẽ không cần cài đặt JDK vào máy thật để chạy app đó, chỉ cần kiếm container đã được setting tương ứng cho app về, bật nó lên, cho app chạy bên trong môi trường container đó, vậy là ok. Khi không sài nữa thì tắt hoặc xóa bỏ container đó đi, không ảnh hưởng gì tới máy thật của bạn.

**Ưu điểm:**

- Linh động: Triển khai ở bất kỳ nơi đâu do sự phụ thuộc của ứng dụng vào tầng OS cũng như cơ sở hạ tầng được loại bỏ.
- Nhanh: Do chia sẻ host OS nên container có thể được tạo gần như một cách tức thì. Điều này khác với vagrant - tạo môi trường ảo ở level phần cứng, nên khi khởi động mất nhiều thời gian hơn.
- Nhẹ: Container cũng sử dụng chung các images nên cũng không tốn nhiều disks.
- Đồng nhất :Khi nhiều người cùng phát triển trong cùng một dự án sẽ không bị sự sai khác về mặt môi trường.
- Đóng gói: Có thể ẩn môi trường bao gồm cả app vào trong một gói được gọi là container. Có thể test được các container. Việc bỏ hay tạo lại container rất dễ dàng.

**Nhược điểm: Tính an toàn**

- Do dùng chung OS nên nếu có lỗ hổng nào đấy ở kernel của host OS thì nó sẽ ảnh hưởng tới toàn bộ container có trong host OS đấy.
- Ngoài ra hãy thử tưởng tượng với host OS là Linux, nếu trong trường hợp ai đấy hoặc một ứng dụng nào đấy có trong container chiếm được quyền superuser, điều gì sẽ xảy ra? Về lý thuyết thì tầng OS sẽ bị crack và ảnh hưởng trực tiếp tới máy host bị hack cũng như các container khác trong máy đó (hacker sử dụng quyền chiếm được để lấy dữ liệu từ máy host cũng như từ các container khác trong cùng máy host bị hack chẳng hạn).

### 1.3. Cài đặt docker

Docker sử dụng công nghệ containerlization, mã nguồn mở.

Docker có 2 phiên bản, CE( dành cho nhà phát triển, nhóm nhỏ, coder như chúng ta) và EE (dành cho doanh nghiệp).

```
I. Chuẩn bị một chút:

# Update the apt package index:
$ sudo apt-get update

# Install packages to allow apt to use a repository over HTTPS:
$ sudo apt-get install \
   apt-transport-https \
   ca-certificates \
   curl \
   software-properties-common

# Add Docker’s official GPG key:
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -


# Verify that you now have the key with the fingerprint 9DC8 5822 9FC7 DD38 854A E2D8 8D81 803C 0EBF CD88, by searching for the last 8 characters of the fingerprint.
$ sudo apt-key fingerprint 0EBFCD88

pub   4096R/0EBFCD88 2017-02-22
     Key fingerprint = 9DC8 5822 9FC7 DD38 854A  E2D8 8D81 803C 0EBF CD88
uid                  Docker Release (CE deb) <docker@docker.com>
sub   4096R/F273FCD8 2017-02-22

# Use the following command to set up the stable repository
$ sudo add-apt-repository \
    "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) \
    stable"
```

```
II. Cài đặt docker CE:

## Update the apt package index:
$ sudo apt-get update

## Install the latest version of Docker CE, or go to the next step to install a specific version. Any existing installation of Docker is replaced.
$ sudo apt-get install docker-ce

## Start and Automate Docker
$sudo systemctl start docker

$sudo systemctl enable docker

## Verify that Docker CE is installed correctly by running the hello-world image.
$ sudo docker run hello-world
```

### 1.4. Cách hoạt động của docker

Docker image là nền tảng của container, có thể hiểu Docker image như khung xương giúp định hình cho container, nó sẽ tạo ra container khi thực hiện câu lệnh chạy image đó. Nếu nói với phong cách lập trình hướng đối tượng, Docker image là class, còn container là thực thể (instance, thể hiện) của class đó.

Docker có hai khái niệm chính cần hiểu, đó là image và container:

- Container: Tương tự như một máy ảo, xuất hiện khi mình khởi chạy image. Tốc độ khởi chạy container nhanh hơn tốc độ khởi chạy máy ảo rất nhiều và bạn có thể thoải mái chạy 4,5 container mà không sợ treo máy. Các files và settings được sử dụng trong container được lưu, sử dụng lại, gọi chung là images của docker.

- Image: Tương tự như file .gho để ghost win mà mấy ông cài win dạo hay dùng. Image này không phải là một file vật lý mà nó chỉ được chứa trong Docker. Một image bao gồm hệ điều hành (Windows, CentOS, Ubuntu, …) và các môi trường lập trình được cài sẵn (httpd, mysqld, nginx, python, git, …). Docker hub là nơi lưu giữ và chia sẻ các file images này (hiện có khoảng 300.000 images). Bạn có thể tìm tải các image mọi người chia sẻ sẵn trên mạng hoặc có thể tự tạo cho mình một cái image như ý.

### 1.5. Các câu lệnh trong docker

`sudo docker pull image_name`: Pull một image từ Docker Hub

`sudo docker run -v <forder_in_computer>:<forder_in_container> -p <port_in_computer>:<port_in_container> -it <image_name> /bin/bash`: Tạo mới container bằng cách chạy image, kèm theo các tùy chọn

`docker images`: Liệt kê các images hiện có

`docker rmi {image_id/name}`: Xóa một image

`docker ps`: Liệt kê các container đang chạy

`docker ps -a`: Liệt kê các container đã tắt

`docker rm -f {container_id/name}`: Xóa một container

`docker start {new_container_name}`: Khởi động một container

`docker exec -it {new_container_name} /bin/bash`: Truy cập vào container đang chạy

### 1.6. Dockerfile là gì?

Dockerfile là một file dạng text, không có đuôi, giúp thiết lập cấu trúc cho docker image nhờ chứa một tập hợp các câu lệnh.

Từ những câu lệnh đó, Docker có thể thực hiện đóng gói một docker images theo yêu cầu tùy biến của riêng bạn.

![](https://images.viblo.asia/751d7512-c9e7-44a5-be56-6b1ff9096adf.png)

Như vậy Dockerfile sẽ quy định Docker image được khởi tạo từ đâu, gồm những gì trong đó.

### 1.7. Cách viết Dockerfile

![](https://images.viblo.asia/53ba5120-21d9-470e-992e-fbcd611b24d2.png)

- Đầu tiên chúng ta sẽ viết Dockerfile để tạo nên image rồi tạo nên container, sau khi tạo được container rồi thì đồng nghĩa là đã tạo ra được máy ảo để bạn có thể khởi chạy ứng dụng của bạn trên máy ảo đó.

- Thư mục webroot chứa mã nguồn chương trình, có thể là một c++ app, java app hoặc web app được viết bằng php hoặc ruby,.... (Ở đây, để cho đơn giản, chúng ta chỉ đặt file hello.html, chạy trên trình duyệt sẽ hiển thị dòng Hello Word). Sau này, bạn dùng editor để lập trình trên máy thật, chỉnh sửa mã nguồn trong thư mục này, mọi sự thay đổi được cập nhật ngay lập tức trên máy ảo.

- File start.sh chứa những câu lệnh được chạy khi bật container (có thể dùng để start mysql, nginx, redis ...)

#### 1.7.1. Thiết lập image gốc

Đầu tiên, ta cần khai báo thằng cha của image này là thằng nào, tức là nó khởi nguồn từ đâu, sử dụng: 
- `FROM`

Image gốc có thể là centos:7, ubuntu:16.04,...

Vi dụ: `FROM ubuntu:16.04`

Có thể bạn sắp biết, Docker hub - nơi lưu trữ và chia sẻ các image sẽ chứa những image gốc mà từ đó, bạn có thể phát triển, cài cắm, thay tháo, chỉnh sửa, thêm bớt để tạo ra những images tùy biến cho riêng bạn.

Khi Docker đọc tới câu lệnh này, nó sẽ tự động tìm xem image ubuntu:16.04 này đã tồn tại trong máy chưa, nếu chưa thì Docker sẽ tự động pull image này về. Trong đó ubuntu là tên của image, 16:04 là tag, bạn cũng có thể hiểu nó nôm na như là branch trong git.

![](https://images.viblo.asia/9194ac19-5ba8-40a6-8f18-ef78f88824b4.png)

- `MAINTAINER` : Một optional dùng để đặt tên cho tác giả của Dockerfile mà bạn đang viết.

 Ví dụ: `MAINTAINER vuongnguyen`

#### 1.7.2. Cài đặt ứng dụng

Bây giờ, chúng ta sẽ cài thêm các ứng dụng, thiết lập môi trường cần thiết trên ubuntu:16.04 này

Bạn có thể cài nginx, php, python, ruby, java ... phụ thuộc vào nhu cầu của bạn, sử dụng:

- `RUN` : Để thực thi một câu lệnh nào đó trong quá trình build images.

- `CMD` : Để thực thi một câu lệnh trong quá trình bật container.

    - Mỗi Dockerfile chỉ có một câu lệnh CMD, nếu như có nhiều hơn một câu lệnh CMD thì chỉ có câu lệnh CMD cuối cùng được sử dụng.

    - Một câu hỏi đặt ra là nếu tôi muốn khởi động nhiều ứng dụng khi start container thì sao, lúc đó hay nghĩ tới ENTRYPOINT

- `ENTRYPOINT`: Để thực thi một số câu lệnh trong quá trình start container, những câu lệnh này sẽ được viết trong file .sh.

VD:

```
# Update ubuntu
RUN apt-get update

# Install nginx
RUN apt-get install -y nginx

# Install mysql server
RUN echo "mysql-server mysql-server/root_password password root" | debconf-set-selections \
    && echo "mysql-server mysql-server/root_password_again password root" | debconf-set-selections \
    && apt-get install -y mysql-server
```

Trong khi cài nginx, sẽ có câu hỏi xuất hiện và bạn cần trả lời yes/no, khi đó tùy chọn -y trong RUN apt-get install -y nginx sẽ thể hiện cho sự lựa chọn yes của bạn.

#### 1.7.3. Cấu hình

- `EXPOSE`: Container sẽ lắng nghe trên các cổng mạng được chỉ định khi chạy

- `ADD`: Copy file, thư mục, remote file thêm chúng vào filesystem của image.

- `COPY`: Copy file, thư mục từ host machine vào image. Có thể sử dụng url cho tập tin cần copy.

- `WORKDIR`: Định nghĩa directory cho CMD

- `VOLUME`: Mount thư mục từ máy host vào container.

Tạo file .sh

```
#!/bin/bash
service nginx start
exec $@
```

Như ở phần entrypoint đã nói, cho dù chỉ cần một câu lệnh mình vẫn dùng ENTRYPOINT, để sau này dễ dàng tùy biến, phát triển.

Ta có ví dụ ở phần này như sau:

```
ADD start.sh /venv

WORKDIR /venv

RUN chmod a+x /venv/*

ENTRYPOINT ["/venv/start.sh"]

EXPOSE 80
```

Tổng hợp lại, ta có một ví dụ cho Dockerfile như sau :

```
FROM ubuntu:16.04

MAINTAINER HoanKi<hoanki2212@gmail.com>

RUN DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get install -y nginx

RUN echo "mysql-server mysql-server/root_password password root" | debconf-set-selections \
    && echo "mysql-server mysql-server/root_password_again password root" | debconf-set-selections \
    && apt-get install -y mysql-server

WORKDIR /venv

COPY start.sh /venv

RUN chmod a+x /venv/*

ENTRYPOINT ["/venv/start.sh"]

EXPOSE 80
```

- Tạo file `hello.html` trong thư mục `webroot`: `<h1>Hello word</h1>`

### 1.8. Cách sử dụng Dockerfile

#### 1.8.1. Build docker image từ Dockerfile

`sudo docker build -t <image_name> .`

VD: `sudo docker build -t ubuntu-nginx . `

#### 1.8.2. Tạo container từ image

`sudo docker run -v <forder_in_computer>:<forder_in_container> -p <port_in_computer>:<port_in_container> -it <image_name> /bin/bash`

Trong đó:

- `-v`: Thể hiện việc mount volume, dữ liệu từ thư mục từ máy thật có thể được truy cập từ thư mục của máy ảo.

- `-p`: Cổng mạng từ máy thật để dẫn tới cổng mạng của máy ảo đang chạy.

- `-t`: Chạy container và mở terminal bằng /bin/bash

Ví dụ vào localhost mặc định của nginx: `sudo docker run -p 9000:80 -it ubuntu-nginx /bin/bash`

Kiểm tra log trên Terminal:

![](https://images.viblo.asia/8708c3a7-45ed-4218-b8b5-e0586a869f9d.png)

Trên trình duyệt:

![](https://images.viblo.asia/8351c336-9101-4344-b626-97193fd48ec8.png)

Ví dụ vào thư mục dự án ở máy thật: `sudo docker run -v  /media/vuongnguyen/PROJECT4/GitRepo/docker_tutorial/webroot:/var/www/html -p 9000:80 -it ubuntu-nginx /bin/bash`

Kết quả:

![](https://images.viblo.asia/08d58efe-8d45-4c5f-aa87-a59b7e96ff80.png)

## 2. Bài tập

Yêu cầu: Viết Dockerfile để container có thể dựa vào image xây dựng từ Dockerfile này và chạy server. Client ở host kết nối tới.

Thư mục mở terminal hiện hành là: `/media/vuongnguyen/DATA/STUDY/Fresher/02-linux-thinking/Ex03-docker`

Thư mục hiện hành chứa: Dockerfile và thư mục data bao gồm:

- `server.c`: code cho server

- `client.c`: code cho client

- `func.c` và `func.h`: các hàm hỗ trợ

- `run-multi-clients`: script để chạy được nhiều client cùng lúc (trong script này là 3 clients)

```
gnome-terminal --command="bash -c 'cd /media/vuongnguyen/DATA/STUDY/Fresher/02-linux-thinking/Ex02-linux-programming/TCP-Server-Client/test; ./client.out a; $SHELL'"
gnome-terminal --command="bash -c 'cd /media/vuongnguyen/DATA/STUDY/Fresher/02-linux-thinking/Ex02-linux-programming/TCP-Server-Client/test; ./client.out b; $SHELL'"
gnome-terminal --command="bash -c 'cd /media/vuongnguyen/DATA/STUDY/Fresher/02-linux-thinking/Ex02-linux-programming/TCP-Server-Client/test; ./client.out c; $SHELL'"
```

- `run-server.sh`: chạy Makefile và run server. Có thể chỉ compile bằng Makefile thôi, không cần run server, tái sử dụng để chạy client trên container luôn.

```
make all
./server.out
```

- `Makefile`: để compile ra chương trình chạy server và client. Mặc dù xây dựng image để chạy server, nhưng tôi viết Makefile compile cả server và client luôn.

```
all: server.out client.out
	echo "Build done!"

server.out: server.o func.o
	gcc server.o func.o -o server.out -pthread

client.out: client.o func.o
	gcc client.o func.o -o client.out

server.o: server.c func.h
	gcc -c server.c

client.o: client.c func.h
	gcc -c client.c

func.o: func.c func.h
	gcc -c func.c

clean:
	rm -f *.o *.out
```

### 2.1. Viết Dockerfile

```
FROM gcc:7.4.0
MAINTAINER  vuongnguyen
RUN mkdir -p /home/data
WORKDIR /home/data
COPY ./data .
RUN apt-get update
CMD chmod +x ./*
ENTRYPOINT ["./run-server.sh"]
ENV PATH /home/data:$PATH
```

- `FROM gcc:7.4.0`: lấy image gốc là gcc:7.4.0
- `MAINTAINER  vuongnguyen`: khai báo chính chủ
- `RUN mkdir -p /home/data`: tạo thư mục /home/data trong image này
- `WORKDIR /home/data`: định nghĩa directory cho thực thi
- `COPY ./data .`: copy các file trong thư mục `data` trên host, vào thư mục hiện hành trong image
- `RUN apt-get update`: update trong image
- `CMD chmod +x ./*`: cấp quyền thực thi cho các file trong WORKDIR
- `ENTRYPOINT ["./run-server.sh"]`: chạy file `run-server.sh` trong WORKDIR
- `ENV PATH /home/data:$PATH`: khai báo biến môi trường

### 2.2. Tạo image từ Dockerfile

- Image mới tên là: `gcc-image:lastest`:

`sudo docker build -t gcc-image:lastest .`

- Tạo và khởi chạy container cho server (tôi dùng foreground) từ image mới build ra, nhớ đặt tên cho container, sử dụng chung đường mạng `host`:

`sudo docker run -it --name=gcc-server --net="host" gcc-image:lastest`

- Ở máy mình (host), có thể dùng script `run-multi-clients.sh` để mở nhiều client kết nối tới container chạy server.

## 3. Nguồn tham khảo

- [Tìm hiểu Docker P1](https://viblo.asia/p/docker-chua-biet-gi-den-biet-dung-phan-1-lich-su-ByEZkWrEZQ0)

- [Tìm hiểu Docker P2](https://viblo.asia/p/docker-chua-biet-gi-den-biet-dung-phan-2-dockerfile-RQqKLzeOl7z)