# Readme for deadline Explorer
### ---------------- INFO ----------------
1. ID: 1512029
2. Name: Trần Quốc Bảo
3. Emal: tranquocbao3897@gmail.com
4. Version Visual Studio: Community 2017 
5. Release: Bản release được build ở dạng release.
### -------------- HAVE DONE --------------
1. Giao diện chương trình. (TreeView, ListView)
2. Xử lý TreeView, ListView, FileSystem.
3. Cụ thể:

> #TreeView
1. Load được ThisPC liệt kê các ổ đĩa hiện có.
2. Bắt được các sự kiện Expanding load folder con.

> #ListView
1. Hiển thị tập tin và thư mục tương ứng với đường dẫn bên TreeView.
2. Bấm đôi vào thư mục sẽ thấy được các thư mục con và tập tin.
3. Có thể thực thi tập tin bằng cách nhấp đôi.
4. Có khả năng hiển thị loại tập tin.
5. Có khả năng hiển thị icon tập tin tương ứng như Windows Explorer.
6. Dùng thư mục ".." để trở về trước giống như phần mềm Total Commander.

> #GUI
1. Sử dụng icon tập tin của hệ thống.
2. Các icon căn bản được set cứng như: các ổ đĩa, thư mục, ThisPC
3. Đồng bộ thay đổi chiều ngang #TreeView với #ListView.

> #Sovle problems
1. Sử dụng link-list để collect các pointer khởi tạo. Tránh gây tràn bộ nhớ cấp phát.
giải quyết được tình trạng chạy nhiều lần sẽ bị treo ứng dụng.
(Giải quyết toàn bộ leak - Dùng Visual Leak Detector for Visual C++ 2008-2015 để kiểm tra)

### -------------- MAIN FLOW --------------
1. Chạy chương trình hiển thị node ThisPC trên TreeView ở trạng thái expand (mở rộng),
gồm các danh sách ổ đĩa hiện có trong máy (không xử lý đĩa mềm :v)
2. Bấm vào ổ đĩa bất kì đang ở trạng thái collapse (thu gọn) bên TreeView thì,
xuất hiện danh sách thư mục con của ổ đĩa. Đồng thời ListView bên phải sẽ hiện thị nội dụng
đầy đủ hơn bao gồm tất cả thư mục và tập tin trong ổ đĩa tương ứng.
3. Đối với các thư mục con ở trạng thái collapse cũng tương tự như vậy.
4. Đồng thời người dùng muốn khởi chạy tập tin như trong windows explorer thì có thể
nhấp đúp chuột vào tập tin để khởi động.
5. Ở ListView, người dùng muốn trở về thư mục cha có thể nhấp đúp chuột vào thư mục ".." giống như Total Commander,
sẽ trở về đến khi nào về lại danh sách ổ đĩa.
### ---------- ADDITIONAL FLOW ------------
Không có
### ------------ LINK BITBUCKET -----------
> https://1512029@bitbucket.org/1512029/explorer.git
```
Chú ý link repository trên là private đã add key public của giáo viên.
```
### ------------- LINK YOUTUBE ------------
> https://youtu.be/lkTBUPsHiHU