.code

next_step proc
; extern "C" void next_step(int count, bool* area, int hight, int width, bool* new_area);
; Параметры:
;	RCX - Размер поля (длина Hight * Width)
;	RDX - Адрес старого массива
;	R8 - Количество точек в высоту
;	R9 - Количество точек в ширину
; Переменные:
;	R10 - Счётчик пройденных точек в массиве
;	R11 - Счётчик живых точек вокруг
;	R12 - Адрес старого массива из RDX
;	R13B - Значение конкретной проверяемой точки
;	R14 - Адрес нового массива

	; Записываем значения регистров Windows в стек
	push r12
	push r13
	push r14

	mov r12, rdx
	mov r14, qword ptr [rsp + 64]
	mov r10, 0
	NextStep:
		; 1. Выход из проверки массива, точки закончились
		cmp r10, rcx
		je ExitLoop

		; 2. Находим значение в данной точке
		mov r13b, byte ptr [r12 + r10]

		; 3. Очищаем счётчик живых точек
		mov r11, 0

		; 4. Выполняем деление Количества пройденных точек (R10) на Количество точек в ширину (R9)
		mov eax, r10d
		xor edx, edx
		div r9d

		; 5. Проверяем наличие соседних точек слева (и сверху и снизу слева)
		cmp edx, 0
		je CheckLeftPoint
		; 6. Проверяем наличие точек соседних точек справа
		dec r9
		cmp edx, r9d
		je CheckRightPoint
		; 7. Проверяем наличие точек соседних точек сверху
		inc r9
		cmp eax, 0
		je CheckTopPoint
		; 8. Проверяем наличие точек соседних точек снизу
		dec r8
		cmp r8, rax
		je CheckBottomPoint
		; 9. Иначе, точка находится в центре
		inc r8
		; Прибавляем верхнюю точку
		mov rax, r10
		sub rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем верхнюю левую точку
		add r11b, byte ptr [r12 + rax - 1]

		; Прибавляем левую точку
		add r11b, byte ptr [r12 + r10 - 1]

		; Прибавляем верхнюю правую точку
		add r11b, byte ptr [r12 + rax + 1]

		; Прибавляем правую точку
		add r11b, byte ptr [r12 + r10 + 1]
		
		; Прибавляем нижнюю точку
		mov rax, r10
		add rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем нижнюю левую точку
		add r11b, byte ptr [r12 + rax - 1]

		; Прибавляем нижнюю правую точку
		add r11b, byte ptr [r12 + rax + 1]

		jmp CheckStatusCurPoint

	CheckBottomPoint:
		inc r8
		; Прибавляем верхнюю точку
		mov rax, r10
		sub rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем верхнюю левую точку
		add r11b, byte ptr [r12 + rax - 1]

		; Прибавляем левую точку
		add r11b, byte ptr [r12 + r10 - 1]

		; Прибавляем верхнюю правую точку
		add r11b, byte ptr [r12 + rax + 1]

		; Прибавляем правую точку
		add r11b, byte ptr [r12 + r10 + 1]
		
		jmp CheckStatusCurPoint

	CheckTopPoint:
		; Прибавляем левую точку
		add r11b, byte ptr [r12 + r10 - 1]

		; Прибавляем нижнюю точку
		mov rax, r10
		add rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем нижнюю левую точку
		add r11b, byte ptr [r12 + rax - 1]

		; Прибавляем правую точку
		add r11b, byte ptr [r12 + r10 + 1]

		; Прибавляем нижнюю правую точку
		add r11b, byte ptr [r12 + rax + 1]
		
		jmp CheckStatusCurPoint

	CheckRightPoint:
		; Проверяем, занимает ли точка верхний правый угол
		inc r9
		cmp eax, 0
		je CheckRightTopPoint
		; Проверяем, занимает ли точка нижний правый угол
		dec r8
		cmp eax, r8d
		je CheckRightBottomPoint
		; Иначе, точка занимает середину правой стороны
		inc r8
		; Прибавляем верхнюю точку
		mov rax, r10
		sub rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем верхнюю левую точку
		add r11b, byte ptr [r12 + rax - 1]

		; Прибавляем левая точку
		add r11b, byte ptr [r12 + r10 - 1]

		; Прибавляем нижнюю точку
		mov rax, r10
		add rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем нижнюю левая точку
		add r11b, byte ptr [r12 + rax - 1]

		jmp CheckStatusCurPoint

	CheckRightBottomPoint:
		inc r8
		; Прибавляем верхнюю точку
		mov rax, r10
		sub rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем верхнюю левую точку
		add r11b, byte ptr [r12 + rax - 1]

		; Прибавляем левую точку
		add r11b, byte ptr [r12 + r10 - 1]
		
		jmp CheckStatusCurPoint


	CheckRightTopPoint:
		; Прибавляем левую точку
		add r11b, byte ptr [r12 + r10 - 1]

		; Прибавляем нижнюю точку
		mov rax, r10
		add rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем нижнюю левую точку
		mov rax, r10
		add rax, r9
		add r11b, byte ptr [r12 + rax - 1]
		
		jmp CheckStatusCurPoint

	CheckLeftPoint:
		; Проверяем, занимает ли точка верхний левый угол
		cmp eax, 0
		je CheckLeftTopPoint
		; Проверяем, занимает ли точка нижний левый угол
		dec r8
		cmp eax, r8d
		je CheckLeftBottomPoint
		; Иначе, точка занимает середину левой стороны
		inc r8
		; Прибавляем верхнюю точку
		mov rax, r10
		sub rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем верхнюю правую точку
		add r11b, byte ptr [r12 + rax + 1]

		; Прибавляем правую точку
		add r11b, byte ptr [r12 + r10 + 1]

		; Прибавляем нижнюю точку
		mov rax, r10
		add rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем нижнюю правую точку
		add r11b, byte ptr [r12 + rax + 1]

		jmp CheckStatusCurPoint

	CheckLeftTopPoint:
		; Прибавляем правую точку
		add r11b, byte ptr [r12 + 1]

		; Прибавляем нижнюю точку
		add r11b, byte ptr [r12 + r9]

		; Прибавляем нижнюю правую точку
		add r11b, byte ptr [r12 + r9 + 1]
		
		jmp CheckStatusCurPoint

	CheckLeftBottomPoint:
		add r8, 1
		; Прибавляем верхнюю точку
		mov rax, r10
		sub rax, r9
		add r11b, byte ptr [r12 + rax]

		; Прибавляем верхнюю правую точку
		add r11b, byte ptr [r12 + rax + 1]

		; Прибавляем правую точку
		add r11b, byte ptr [r12 + r10 + 1]
		
		jmp CheckStatusCurPoint
	
	CheckStatusCurPoint:
		cmp r13, 1
		je CheckAlive
		jmp CheckDead

	CheckAlive:
		cmp r11, 2
		je KeepAlive
		cmp r11, 3
		je KeepAlive
		
		mov byte ptr [r14 + r10], 0
		jmp Increment
	KeepAlive:
		mov byte ptr [r14 + r10], 1
		jmp Increment

	CheckDead:
		cmp r11, 3
		jne KeepDead
		
		mov byte ptr [r14 + r10], 1
		jmp Increment
	KeepDead:
		mov byte ptr [r14 + r10], 0
		jmp Increment

	Increment:
		inc r10
		jmp NextStep

	ExitLoop:
		pop r14
		pop r13
		pop r12
		ret
next_step endp

end