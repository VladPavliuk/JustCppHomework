bits 16

mov si, bx
mov dh, al

mov cx, 12
mov cx, -12

mov dx, 3948
mov dx, -3948

mov al, [bx + si]
mov bx, [bp + di]
mov dx, [bp]

mov ah, [bx + si + 4]
mov al, [bx + si + 4999]

mov [bx + di], cx
mov [bp + si], cl
mov [bp], cl

mov ax, [bx + di - 37]
mov [si - 300], cx
mov dx, [bx - 32]

mov [bp + di], byte 7
mov [di + 901], word 347

mov bp, [5]
mov bx, [3458]

mov ax, [2555]
mov ax, [16]

mov [2554], ax
mov [15], ax


add bx, [bx+si]
add bx, [bp]
add si, 2
add bp, 2
add cx, 8
add bx, [bp + 0]
add cx, [bx + 2]
add bh, [bp + si + 4]
add di, [bp + di + 6]
add [bx+si], bx
add [bp], bx
add [bp + 0], bx
add [bx + 2], cx
add [bp + si + 4], bh
add [bp + di + 6], di
add byte [bx], 34
add word [bp + si + 1000], 29
add ax, [bp]
add al, [bx + si]
add ax, bx
add al, ah
add ax, 1000
add al, -30
add al, 9

sub bx, [bx+si]
sub bx, [bp]
sub si, 2
sub bp, 2
sub cx, 8
sub bx, [bp + 0]
sub cx, [bx + 2]
sub bh, [bp + si + 4]
sub di, [bp + di + 6]
sub [bx+si], bx
sub [bp], bx
sub [bp + 0], bx
sub [bx + 2], cx
sub [bp + si + 4], bh
sub [bp + di + 6], di
sub byte [bx], 34
sub word [bx + di], 29
sub ax, [bp]
sub al, [bx + si]
sub ax, bx
sub al, ah
sub ax, 1000
sub al, -30
sub al, 9

cmp bx, [bx+si]
cmp bx, [bp]
cmp si, 2
cmp bp, 2
cmp cx, 8
cmp bx, [bp + 0]
cmp cx, [bx + 2]
cmp bh, [bp + si + 4]
cmp di, [bp + di + 6]
cmp [bx+si], bx
cmp [bp], bx
cmp [bp + 0], bx
cmp [bx + 2], cx
cmp [bp + si + 4], bh
cmp [bp + di + 6], di
cmp byte [bx], 34
cmp word [4834], 29
cmp ax, [bp]
cmp al, [bx + si]
cmp ax, bx
cmp al, ah
cmp ax, 1000
cmp al, -30
cmp al, 9

test_label0:
jnz test_label1
jnz test_label0
test_label1:
jnz test_label0
jnz test_label1