# Docker

## Deploying Your First Docker Container

### Step 1 - Running A Container

`docker search <name>` : Tìm image có sẵn trên docker hub

VD: `docker search redis`

`docker run <options> <image-name>` : Khởi động container dựa trên image

Mặc định, docker chạy command trên foreground. Để chạy background, dùng option `-d`.

VD: `docker run -d redis`

Mặc định, docker sẽ chạy phiên bản mới nhất có sẵn. Có thể chỉ định phiên bản:
`docker run -d redis:3.2`

`docker run -d redis:latest`

### Step 2 - Finding Running Containers

Container đang chạy trên background, lệnh `docker ps` sẽ liệt kê list tất cả container đang chạy, image mà nó sử dụng và thời gian hoạt động. Nó cũng hiển thị tên và ID cũng từng container.

`docker inspect <friendly-name|container-id>` : cung cấp thêm chi tiết về một container, như địa chỉ IP.

`docker logs <friendly-name|container-id>` : hiển thị thông báo mà container đã ghi thành standard error hoặc standard out.

### Step 3 - Accessing Redis

Học cách truy cập container.

Mặc định, Redis chạy trên port 6379.

Sử dụng option `-p <host-port>:<container-port>`

`docker run -d --name redisHostPort -p 6379:6379 redis:latest` : Chạy container ở background với tên là redisHostPort, ánh xạ host port 6379 tới container port 6379.

Mặc định, port trên host ánh xạ tới 0.0.0.0 nghĩa là tất cả các địa chỉ IP. Ta có thể chỉ định một địa chỉ IP cụ thể khi xác định port, ví dụ: `-p 127.0.0.1:6379:6379`.

### Step 4 - Accessing Redis

Vấn đề với việc đang chạy process trên một port cố định thì ta chỉ có thể chạy một instance. Giờ ta muốn chạy nhiều instance Redis và định cấu hình ứng dụng tùy thuộc vào port Redis đang chạy.

Ta sử dụng option `-p 6379` cho phép ta tiếp xúc với Redis trên một port có sẵn ngẫu nhiên

`docker run -d --name redisDynamic -p 6379 redis:latest` : Chạy container trên background với tên là redisDynamic, ánh xạ host port ngẫu nhiên tới port 6379 của container.

Giờ để xác định host port ngẫu nhiên được chỉ định, dùng `docker port redisDynamic 6379`

### Step 5 - Persisting Data

Dữ liệu được lưu sẽ mất nếu xóa và tạo lại container. Cần duy trì và sử dụng lại khi tạo lại container.

Các thư mục liên kết (volume) được thực hiên bằng `-v <host-dir>:<container-dir>`. Khi thư mục được gắn kết, các file tồn tại trong thư mục đó trên host được truy cập bởi container và mọi sự thay đổi dữ liệu trong thư mục của container sẽ được lưu trữ trên host.

Ví dụ: `docker run -d --name redisMapped -v /opt/docker/data/redis:/data redis`

Docker cho phép sử dụng $PWD làm biến cho thư mục hiện tại

`docker run -d --name redisMapped -v "$PWD/data":/data redis`

### Step 6 - Running A Container In The Foreground

Làm thế nào để container làm việc với foreground, như `ps` hoặc `bash`.

Nếu như muốn tương tác với container (ví dụ như truy cập bash shell), dùng tùy chọn `-it` để foreground.

`docker run ubuntu ps` : khởi chạy Ubuntu container và thực thi lệnh `ps` để xem tất cả các process đang chạy trong container

`docker run -it ubuntu bash` : cho phép ta có quyền truy cập vào bash shell trong container

## Deploy Static HTML Website as Container

Tạo Docker Image để chạy trang web HTML tĩnh bằng Nginx. Tìm hiểu cách tạo ra Docker Image the ý chính mình,

### Step 1 - Create Dockerfile

Docker Image bắt đầu từ một image cơ sở. Image cơ sở nên bao gồm platform dependencies cần thiết cho ứng dụng, như: JVM, CLR.

Dockerfile là một danh sách các hướng dẫn mô tả cách triển khai ứng dụng của bạn.

Trong ví dụ này, image cơ sở của ta là Alpine version của Nginx.

Tạo Dockerfile để xây dựng image bằng cách dùng nội dung bên dưới.

```
FROM nginx:alpine
COPY . /usr/share/nginx/html
```

- Image cơ sở: nginx:alpine
- Sao chép nội dung trong thư mục hiện tại tới thư mục `/usr/share/nginx/html` của container

### Step 2 - Build Docker Image

Dockerfile được dùng bởi lệnh `build` trên Docker CLI. Lệnh `build` thực thi từng dòng các lệnh trong Dockerfile. Kết quả là một Docker Image được tạo ra.

`docker build -t <build-directory>` : chỉ định tên cho image và tag

Ví dụ: Build một static HTML image
`docker build -t webserver-image:v1 .` 

Xem list image có trên host: `docker images`

### Step 3 - Run

Sử dụng image vừa được build:

`docker run -d -p 80:80 webserver-image:v1`

## Building Container Images

Với section này, container sẽ chạy static HTML application sử dụng Nginx. Các section sau, sẽ giới thiệu cách triển khai Node.js hoặc ASP.NET.

Docker images được xây dựng dựa trên Dockerfile. Dockerfile xác định tất cả các bước cần thiết để tạo Docker image với ứng dụng của bạn được cấu hình và sẵn sàng để chạy như một container. Image chứa tất cả mọi thứ, từ hệ điều hành đến dependencies và configuretion cần thiết để chạy ứng dụng của bạn.

### Step 1 - Base Images

Những image cơ sở được sử dụng làm nền tảng cho các thay đổi bổ sung để chạy ứng dụng. Ví dụ: trong kịch bản này, chúng ta yêu cầu NGINX được cấu hình và chạy trên hệ thống trước khi chúng ta có thể triển khai các file HTML tĩnh của mình. Vì vậy, chúng ta muốn sử dụng NGINX làm image cơ bản.

Dockerfile là các file text đơn giản với một lệnh trên mỗi dòng. Để xác định image cơ sở, ta sử dụng `FROM <image-name>:<tag>`

VD: `FROM nginx:1.11-alpine`

Nên sử dụng phiên bản cụ thể làm tag và tự quản lý việc cập nhật.

### Step 2 - Running Commands

`RUN <command>` : cho phép ta thực hiện bất kỳ lệnh nào như là thực hiện trên command prompt

`COPY <src> <dest>` : cho phép ta sao chép các file từ thư mục chứa Dockerfile sang image của container

VD: `COPY index.html /usr/share/nginx/html/index.html`

### Step 3 - Exposing Ports

Các file được sao chép vào image và bất kỳ dependencies được tải nào, ta cần xác định port cần được truy cập.

`EXPOSE <port>` : khai báo với Docker, port được mở và cũng có thể bị ràng buộc. Có thể xác định nhiều port `EXPOSE 80 433` hoặc `EXPOSE 7000-8000`

### Step 4 - Default Commands

`CMD` : xác định lệnh mặc định để chạy khi container khởi chạy. Nếu lệnh yêu cầu nhiều tham số, ta dùng mảng, ví dụ: `["cmd", "-a", "arga value", "-b", "argb-value"]` và lệnh `cmd -a "arga value" -b argb-value` sẽ được chạy.

VD: Lệnh chạy NGINX là `nginx -g daemon off;` sẽ được biểu diễn là: `CMD ["nginx", "-g", "daemon off;"]`

Ta có thể dùng `ENTRYPOINT`. Mặc dù `CMD` có thể được ghi đè khi container khởi chạy, `ENTRYPOINT` là một lệnh mà có thể có nhiều đối số khi container khởi chạy.

### Step 5 - Building Containers

Sau khi viết Dockerfile, dùng `docker build` để biến nó thành image. 

Có thể dùng option `-t <name>`

VD: `docker build -t my-nginx-image:latest .`

### Step 6 - Launching New Image

`docker run -d -p 80:80 <image-id|friendly-tag-name>` : khởi chạy container trên background, web server truy cập và blind nó tới port 80.

VD: `docker run -d -p 80:80 my-nginx-image:latest`

Có thể truy cập khởi chạy web server thông qua hostname `docker`. Sau khi khởi chạy container, dùng `curl -i http://docker` trả về file index thông qua NGINX và image chúng ta đã build.

Dùng `docker ps` để kiểm tra các container đang chạy.

## Dockerizing Node.js applications

Ta tiếp tục khám phá cách xây dựng và triển khai ứng dụng bằng docker container. Lần này, ta học cách triển khi ứng dụng Node.js trong container.

Môi trường được cấu hình với quyền truy cập Docker instance cá nhân và code cho ExpressJS trng working directory. Dùng `ls` và `cat <filename>` để xem code hoặc dùng editor.

Nếu muốn truy cập service, sử dụng `docker` thay vì localhost hoặc 0.0.0.0

### Step 1 - Base Image

Ta dùng image cơ sở: node:10-alpine

Dùng `RUN <command>` với các lệnh như `mkdir` để tạo ra thư mục nơi ứng dụng sẽ thực thi từ đó.

Xác định working directory bằng `WORKDIR <directory>` để đảm bảo **tất cả các lệnh** trong tương lai được thực thi từ thư mục liên qua đến ứng dụng của chúng ta.

VD:

```
FROM node:10-alpine

RUN mkdir -p /src/app

WORKDIR /src/app
```

### Step 2 - NPM Install

Cài đặt dependencies cần thiết để chạy ứng dụng. Với Node.js thì cần cài NPM.

Để giữ thời gian xây dựng ở mức tối thiểu, Docker lưu trữ kết quả thực hiện một dòng trong Dockerfile để sử dụng trong quá trình build trong tương lai. Nếu một cái gì đó đã thay đổi, thì Docker sẽ làm mất hiệu lực dòng hiện tại và tất cả các dòng sau để đảm bảo mọi thứ đều được cập nhật.

VD: Với NPM, chúng ta chỉ muốn chạy lại `npm install` nếu có gì đó trong file `pack.json` của chúng ta đã thay đổi. Nếu không có gì thay đổi thì chúng ta có thể sử dụng cache version để tăng tốc độ triển khai. Bằng cách sử dụng `COPY pack.json <Dest>`, chúng ta có thể khiến lệnh `RUN npm install` bị vô hiệu nếu file `pack.json` đã thay đổi. Nếu tệp không thay đổi, thì cache sẽ không bị vô hiệu và kết quả được lưu trong bộ nhớ cache của lệnh `npm install` sẽ được sử dụng.

```
COPY package.json /src/app/package.json

RUN npm install
```

Nếu bạn không muốn sử dụng cache như một phần của bản build thì hãy đặt option `--no-cache = true` như một phần của lệnh `docker build`.

### Step 3 - Configuring Application

VD:

```
COPY . /src/app

EXPOSE 3000

CMD [ "npm", "start" ]

```

Các câu lệnh command sẽ chạy trong thư mục WORKDIR

### Step 4 - Building & Launching Container

Để khởi chạy ứng dụng trong container, đầu tiên ta cần build image:

`docker build -t my-nodejs-app .`

Khởi chạy image đã được build:

`docker run -d --name my-running-app -p 3000:3000 my-nodejs-app`

Test Container:

`curl http://docker:3000`

### Step 5 - Environment Variables

Docker images nên được thiết kế để chúng có thể được chuyển từ môi trường này sang môi trường khác mà không thực hiện bất kỳ thay đổi hoặc yêu cầu build lại.

Với Docker, các biến môi trường có thể được xác định khi ta khởi chạy container. Ví dụ: với các ứng dụng Node.js, ta nên xác định biến môi trường cho `NODE_ENV` khi chạy.

Option `-e` có thể đặt tên và giá trị là `-e NODE_ENV=production`.

VD: 

```
docker run -d --name my-production-running-app -e NODE_ENV=production -p 3000:3000 my-nodejs-app
```

## Optimising Builds With Docker OnBuild

Tối ưu Dockerfile sử dụng OnBuild instruction.

Môi trường được cấu hình như ví dụ ứng dụng Node.js, tuy nhiên, các tiếp cận có thể được áp dụng cho bất kỳ image nào. 

### Step 1 - OnBuild

Dockerfile thực thi từ trên xuống, ta có thể kích hoạt một lệnh được thực thi sau đó khi image được dùng làm image cơ sở cho image khác.

Kết quả là ta có thể trì hoãn việc thực thi của mình để phụ thuộc vào ứng dụng mà ta đang xây dựng, ví dụ như file pack.json của ứng dụng.

Dưới đây là Node.js OnBuild Dockerfile. Không giống như trong kịch bản trước đây của chúng ta, ứng dụng chỉ định các lệnh đã thêm tiền tố ONBUILD.

```
FROM node:7
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
ONBUILD COPY package.json /usr/src/app/
ONBUILD RUN npm install
ONBUILD COPY . /usr/src/app
CMD [ "npm", "start" ]
```

Kết quả là chúng ta có thể build image này nhưng các lệnh cụ thể của ứng dụng sẽ không được thực thi cho đến khi cái image được build được sử dụng làm image cơ sở. Sau đó, chúng sẽ được thực thi như là một phần của bản build của image cơ sở.

### Step 2 - Application Dockerfile

Ưu điểm của việc tạo OnBuild images là Dockerfile của chúng ta giờ đây đơn giản hơn nhiều và có thể dễ dàng sử dụng lại trên nhiều dự án mà không phải chạy lại các bước tương tự cải thiện thời gian build.

```
FROM node:7-onbuild
EXPOSE 3000
```

### Step 3 - Building & Launching Container

Build image dựa trên OnBuild docker file giống với những cái trước. Lệnh OnBuild sẽ được thực thi như thể chúng ở trong Dockerfile cơ sở.

`docker build -t my-nodejs-app .`

`docker run -d --name my-running-app -p 3000:3000 my-nodejs-app`

Test conainer:

`curl http://docker:3000`

## Ignoring Files During Build

Chúng ta có thể bỏ qua các file nhất định khỏi Docker image mà có thể gây rủi ro bảo mật. Chúng ta học cách giảm thời gian build bằng cách bỏ qua các file được gửi tới Docker Built Context.

### Step 1 - Docker Ignore

Để ngăn các file hoặc thư mục nhạy cảm trong image, ta có thể thêm một file có tên `.dockerignore`.

Dockerfile sao chép working directory vào Docker Image. Do đó, điều này sẽ bao gồm thông tin nhạy cảm như file mật khẩu mà chúng ta muốn quản lý bên ngoài image.

Giải quyết bằng cách đưa file mật khẩu vào `.dockerignore` bằng lệnh `echo passwords.txt >> .dockerignore`.

Ignore file cũng hỗ trợ bỏ qua thư mục và regular expression giống như `.gitignore`. Nó có thể giúp cải thiện thời gian build.

Nếu ta cần sử dụng mật khẩu trong lệnh RUN thì ta cần copy, thực thi và xóa file. Chỉ trạng thái cuối cùng của Docker container được duy trì trong image.

### Step 2 - Docker Build Context

Giả sử trong môi trường, một file tạm 100M được tạo ra. File này không bao giờ được Dockerfile sử dụng. Khi ta thực thi lệnh build, Docker gửi toàn bộ nội dung đường dẫn đến Engine để tính các file nào  được include. Kết quả là việc gửi file 100M không được yêu cầu và tạo ra một bản build chậm.

Chúng ta nên bỏ qua các thư mục .git cùng với dependencies được download/build trong image như node_modules. Chúng không bao giờ được sử dụng bởi ứng dụng chạy trong Docker Container và làm tăng chi phí cho quá trình build.

### Step 3 - Optimised Build

Chúng ta đã sử dụng file `.dockerignore` để loại trừ các file nhạy cảm, chúng ta có thể sử dụng file này để loại trừ các file mà chúng ta không muốn gửi đến Docker Build Context trong quá trình build.

Để tăng tốc độ build, chỉ cần include tên của cái file lớn trong ignore file.

VD: `echo big-temp-file.img >> .dockerignore`

Khi chúng ta build lại image, nó sẽ nhanh hơn nhiều vì nó không phải sao chép file 100M.

`docker build -t no-large-file-context .`

Tối ưu hóa này có tác động lớn hơn khi bỏ qua các thư mục lớn như `.git`.

## Create Data Containers

Dùng option `-v <host-dir>:<container-dir>` để ánh xạ các thư mục giữa host và container. Hoặc cách tiếp cận khác là dùng Data Containers.

### Step 1 - Create Container

Data Containers là những container có trách nhiệm duy nhất là nơi lưu trữ/quản lý dữ liệu. Chúng không chạy nên khi ta thực hiện `docker ps` sẽ không liệt kê ra.

VD: Chọn `busybox` làm cơ sở vì nó nhỏ và nhẹ trong trường hợp chúng ta muốn khám phá và di chuyển container tới host khác.

Khi tạo container, dùng `-v` để xác định nơi các container khác sẽ đọc/ghi dữ liệu.

VD: Tạo Data container để lưu trữ configuration files dùng: `docker create -v /config --name dataContainer busybox` với nơi sẽ được các container khác đọc/ghi dữ liệu là thư mục `/config`.

### Step 2 - Copy Files

Có thể copy file từ local client directory vào container dùng lệnh `docker cp`.

VD: Copy config.conf vào thư mục config của `dataContainer` vừa tạo ở bước trên.

`docker cp config.conf dataContainer:/config/`

### Step 3 - Mount Volumes From

Bây giờ, data container có config của ta, chúng ta có thể tham chiếu container khi chúng ta khởi chạy các dependent container mà yêu cầu configuration file.

Dùng option `--volumes-from <container>` để mount volume từ container khác vào container đang khởi chạy. Ví dụ như trong trường hợp này, chúng ta đang khởi chạy Ubuntu container tham chiếu tới Data container của chúng ta. Vậy khi list config directory, nó sẽ hiển thị các file từ container đính kèm.

`docker run --volumes-from dataContainer ubuntu ls /config`

Nếu thư mục `/config` tồn tại thì `volumes-from` sẽ override nó. Có thể ánh xạ nhiều volume tới một container.

### Step 4 - Export / Import Containers

Nếu muốn di chuyển data container sang một máy khác, chúng ta có thể export nó ra `.tar` file.

VD: `docker export dataContainer > dataContainer.tar`

Import Data container trở vào Docker: `docker import dataContainer.tar`

## Creating Networks Between Containers using Links

Bài này học cách cho phép nhiều container giao tiếp với nhau. Cụ thể ta tìm hiểu cách kết nối data-store như Redis tới một ứng dụng đang chạy trong một container riêng biệt.

Môi trường được config với docker client và daemon. Tên máy mà docker daemon đang chạy gọi là docker.

### Step 1 - Start Redis

Trường hợp phổ biến để kết nối các container là ứng dụng kết nối với data-store. 

Chạy redis server với tên là `redis-server` nơi mà chúng ta sẽ kết nối tới. Đây là source container.

`docker run -d --name redis-server redis`

### Step 2 - Create Link

Để kết nối tới source container, dùng option `--link <container-name|id>:<alias>` khi khởi chạy mội container mới. Với container-name là tên của source container. Còn đặt alias, app config không cần thay đổi khi được kết nối với môi trường khác.

Ví dụ này, ta sẽ tạo một Alpine container để link tới `redis-server` mà ta đã tạo. Ta đặt alias là redis. Khi link được tạo, Docker sẽ làm 2 việc:

- Đầu tiên, Docker sẽ thiết lập một số biến môi trường dựa trên liên kết đến container. Các biến môi trường này cung cấp cách để tham chiếu thông tin như Port và địa chỉ IP thông qua các tên đã biết.

VD: Để xuất các biến môi trường dùng lệnh `env`

`docker run --link redis-server:redis alpine env`

- Docker sẽ cập nhật HOSTS file của container với một entry cho source container, với 3 tên: original, alias, hash-id.

VD: Để xuất container host entry:

`docker run --link redis-server:redis alpine cat /etc/hosts`

Giờ thử ping tới source container: `docker run --link redis-server:redis alpine ping -c 1 redis`

### Step 3 - Connect To App

Lấy ví dụ một ứng dụng node.js kết nối tới redis dùng hostname redis:

`docker run -d -p 3000:3000 --link redis-server:redis katacoda/redis-node-docker-example`

Để test connection, ta gửi HTTP request tới ứng dụng sẽ lưu request trong Redis và trả về một số đếm. Nếu khi có nhiều request, ta sẽ thấy biến đếm tăng:

`curl docker:3000`

### Step 4 - Connect to Redis CLI

Chúng ta có thể kết nối source container tới ứng dụng, ta cũng có thể kết nối chúng tới CLI tool.

VD: Khởi chạy một thể hiện của Redis CLI và kết nối tới redis server thông qua alias. Lệnh này có thể hiểu là: chúng ta đã khởi chạy redis-server container (alias là redis), sau đó ta tiếp tục khởi chạy container mới (với image redis và trên foreground) liên kết tới redis-server và gọi redis-cli để giao tiếp với server. Vậy client và server của redis chạy trên 2 container khác nhau và đã được liên kết với nhau.

`docker run -it --link redis-server:redis redis redis-cli -h redis`

## Creating Networks Between Containers using Networks

Ở bài này, ta sẽ tìm hiểu cách tạo docker netword cho phép các container giao tiếp. Ta cũng tìm hiểu Embedded DNS Server trong Docker 1.10

Docker có 2 cách tiếp cận để networking. 

- Cách đầu là xác định link giữa các container. Link này cập nhật /etc/hosts và biến môi trường cho phép các container giao tiếp với nhau.

- Cách khác là tạo ra một docker network để các container kết nối tới. Network ở đây tương tự như physical network, cho phép các container đến và đi tự do hơn so với khi dùng cách xác định link.

### Step 1 - Create Network

Tạo network với tên xác định:

`docker network create backend-network`

Dùng `--net` để kết nối container đến network:

`docker run -d --name=redis --net=backend-network redis`

### Step 2 - Network Communication

Docker network giống như traditional network nơi các node có thể attached/detached.

Docker không còn gán biến môi trường hoặc cập nhật hosts file. Để xác nhận điều đó, ta dùng 2 lệnh sau:

`docker run --net=backend-network alpine env`

`docker run --net=backend-network alpine cat /etc/hosts`

Thay vào đó, các container giao tiếp thông qua một Embedded DNS Server trong Docker. DNS Server này được gán cho tất cả các container thông qua IP 127.0.0.11 và đặt trong `resolv.conf` file.

`docker run --net=backend-network alpine cat /etc/resolv.conf`

Khi các container cố truy cập vào container khác, DNS Server sẽ trả về địa chỉ IP của container. Trong trường hợp này, kết quả trả về là `redis.backend-network`.

`docker run --net=backend-network alpine ping -c1 redis`

### Step 3 - Connect Two Containers

Docker hỗ trợ nhiều network và container attach nhiều network tại cùng một thời điểm.

VD: Tạo 1 network riêng biệt với 1 Node.js app gao tiếp với 1 thể hiện Redis đã tồn tại:

- Đầu tiên, tạo 1 network mới:

`docker network create frontend-network`

- Lệnh `connect` có thể attach container đã tồn tại tới network: Nó sẽ kết nối container `redis` đã tạo (đã được kết nối với `backend-network`) ở trên với network `frontend-network`.

`docker network connect frontend-network redis`

- Khi khởi chạy web server, do được đưa vào cùng network, nó sẽ có thể giao tiếp với thể hiện Redis của chúng ta.

`docker run -d -p 3000:3000 --net=frontend-network katacoda/redis-node-docker-example`

- Test: `curl docker:3000`

### Step 4 - Create Aliases

Cung cấp một alias khi kết nối một container tới một network.

 
 
 
 
`docker network connect --alias db frontend-network2 redis`

Khi container truy cập service theo tên `db`, chúng sẽ được cung cấp địa chỉ IP của Redis container.

`docker run --net=frontend-network2 alpine ping -c1 db`

### Step 5 - Disconnect Containers

Liệt kê tất cả các network:

`docker network ls`

Chúng ta có thể  tìm thấy container nào được attach và địa chỉ IP của chúng:

`docker network inspect frontend-network`

Lệnh ngắt kết nối redis container khỏi front-end-network:

`docker network disconnect frontend-network redis`

## Persisting Data Using Volumes

Học cách sử dụng Docker Volumes để duy trì dữ liệu trong các Container. Docker Voumes cho phép các thư mục được chia sẻ giữa các container và các container version.

Docker Voumes cho phép nâng cấp các container, restart machine và chia sẻ dữ liệu mà không mất dữ liệu. Điều này là cần thiết khi cập nhật database hoặc application version.

### Step 1 - Data Volumes

Docker Volumes được tạo và gán khi các container khởi động. Nó cho phép ánh xạ host directory tới container để chia sẻ dữ liệu.

Việc ánh xạ này là 2 chiều. Dữ liệu lưu ở host được truy cập từ trong các container và dữ liệu được lưu trữ trong container được lưu trên host.

VD: Dùng redis để duy trì dữ liệu: /docker/redis-data trên host và /data trên container, tên container là r1, chạy background, image là redis, chạy command: redis-server --appendonly yes

`docker run  -v /docker/redis-data:/data --name r1 -d redis redis-server --appendonly yes`

Ta có thể dẫn dữ liệu (pipe data) vào một thể hiện của Redis với lệnh:

`cat data | docker exec -i r1 redis-cli --pipe`

Redis sẽ lưu dữ liệu vào đĩa. Trên host, ta có thể xem trực tiếp ánh xạ sẽ chứa Redis data file.

`ls /docker/redis-data`

Có thể dùng cùng một thư mục trên host để ánh xạ với một thư mục của container khác:

`docker run -v /docker/redis-data:/backup ubuntu ls /backup`

### Step 2 - Shared Volumes

Data Voumes ánh xạ tới host để duy trì dữ liệu. Tuy nhiên, để có được quyền truy cập vào chúng từ một container khác, ta cần phải biết đường dẫn tuyệt đối.

Thay vào đó, ta có thể dùng `-volumes-from`. Nó sẽ ánh xạ từ source container tới container đang được khởi chạy.

VD: Ánh xạ volume của Redis container tới Ubuntu container. Thư mục /data chỉ tồn tại trong Redis container, với `-volumes-from` thì Ubuntu container có thể truy cập dữ liệu.

`docker run --volumes-from r1 -it ubuntu ls /data`

Vậy ta có thể truy cập các volume từ container khác mà không cần phải quan tâm đến việc chúng có trên host chưa.

### Step 3 - Read-only Volumes

Mount Volumes cung cấp cho container đầy đủ quyền đọc ghi tới thư mục. Ta có thể chỉ định quyền chỉ đọc trên thư mục bằng cách thêm `:ro` vào mount.

Nếu container cố sửa đổi dữ liệu trong thư mục thì sẽ báo lỗi.

`docker run -v /docker/redis-data:/data:ro -it ubuntu rm -rf /data` => lỗi





