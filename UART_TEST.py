import serial,time
ser = serial.Serial('COM10', 9600)


GatewayID = 234



def sendMessage(mes):
    #Отправка сообщения чисто для
	ser.write(bytes(mes))
	#ser.close()




def parseMessage(mes):

	print(bytes(mes))
	messageID = mes[0]  # Номер посылки
	nodeID = mes[1]   #id узла от которого пришло сообщение
	recieverID = mes[2] #id получателя. Для шлюза все равно.
	retFlag = mes[3] #Метка ретранслировано ли сообщение
	payload = mes[4:-3]
	endbyte = mes[-3:]

	print("messageID",messageID)
	print("nodeID", nodeID)
	print("recieverID", recieverID)
	print("retFlag", retFlag)
	print("payload", payload)
	print("endbyte", endbyte)


    # Понять сообщение целое не целое
    ###################################
	# Ответить на сообщение (Я принял)
	# Пишем сообщение для отпрвки (все тоже самое что пришло только споменяные айдишники)
	newMes = []
	newMes.append(messageID)
	newMes.append(GatewayID)
	newMes.append(nodeID)
	newMes.append(retFlag)
	newMes.extend(payload)
	newMes.extend(endbyte)

	sendMessage(newMes)
	print(bytes(newMes))



mess = []
try:
	#sendMessage("asd")
	while 1:
		# ловим сообщение и ищем конец
		s = ser.read()
		mess += s
		#parseMessage(s)
		if (mess[-3:] == [0xFF,0xFE,0xFF]):
			parseMessage(mess)
			mess.clear()






except KeyboardInterrupt:
	pass


#ser.write(bytes([1,2,3,4,5,6,7,8]))