numero = int(input())

inst = 3
i=2
ehPrimo = True

while i < numero/2:
    inst += 1
    inst += 1
    if numero % i == 0:
        ehPrimo=False
        inst += 1
    i+=1
    inst += 1

inst += 1
if ehPrimo and numero != 1:
    print(numero, "eh primo\n")
else:
    print(numero, "não eh primo\n")
inst+=1

print("Número: ", numero, " Instruções: ", inst)