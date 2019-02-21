import matplotlib.pyplot as plt
import numpy as np


h_sample,b_sample = [] , []
count = -1
buff_size = 0
xlabel = []
h_conf = []	# conflict
b_st,h_st,h_ft = [],[],[]	# searchTime
h_ct,b_ct = [],[] # ConstructTime
h_cor,b_cor = [], [] # Correct

with open("logfile/hash_log","r") as f:
	lines = f.readlines()
	for line in lines:
		if "Sample Num" in line:
			count = count + 1
			data = {}
			h_sample.append(data)
			sample_num = line.split(" ")[3]
			h_sample[count]['sample_num'] = sample_num
		elif "Buffer size" in line:
			buff_size = int(line.split(" ")[3])
			
		elif "Conflicts" in line:
			conflict = line.split(" ")[2]
			h_sample[count]['conflict'] = float(conflict)/float(h_sample[count]['sample_num'])*100

		elif "Search Time" in line:
			search_time = line.split(" ")[3]
			h_sample[count]['search_time'] = float(search_time)

		elif "Function Time" in line:
			function_time = line.split(" ")[3]
			h_sample[count]['function_time'] = float(function_time)

		elif "constructing time" in line:
			construct_time = line.split(" ")[3]
			h_sample[count]['construct_time'] = float(construct_time)
		elif "Correct Percent" in line:
			correct = line.split(" ")[3]
			h_sample[count]['correct_per'] = float(correct)


count = -1

with open("logfile/binary_log","r") as f:
	lines = f.readlines()
	for line in lines:
		if "Sample Num" in line:
			count = count + 1
			data = {}
			b_sample.append(data)
			sample_num = line.split(" ")[3]
			b_sample[count]['sample_num'] =  sample_num

		elif "Search Time" in line:
			search_time = line.split(" ")[3]
			b_sample[count]['search_time'] = float(search_time)	

		elif "constructing time" in line:
			construct_time = line.split(" ")[3]
			b_sample[count]['construct_time'] = float(construct_time)
		elif "Correct Percent" in line:
			correct = line.split(" ")[3]
			b_sample[count]['correct_per'] = float(correct)


for data in h_sample:
	xlabel.append(data['sample_num'])
	h_conf.append(data['conflict'])
	h_st.append(data['search_time'])
	h_ft.append(data['function_time'])
	h_ct.append(data['construct_time'])	
	h_cor.append(data['correct_per'])

for data in b_sample:
	b_st.append(data['search_time'])
	b_ct.append(data['construct_time'])
	b_cor.append(data['correct_per'])

plt.figure(figsize=(12,12))
plt.subplot(221)
x = np.arange(len(xlabel))
plt.title("Conflict rate ({})".format(buff_size))
plt.bar(x,h_conf)
plt.xticks(x,xlabel)
plt.ylabel("Conflict( %)")

plt.subplot(222)
plt.title("Search Time")
plt.plot(x,h_st,'rs--')
plt.plot(x,h_ft,'gs--')
plt.plot(x,b_st,'bs--')
plt.xticks(x,xlabel)
plt.ylabel("Search Time(seconds)")
plt.legend(['hash_search','has_function','binary'])
plt.grid()

plt.subplot(223)
plt.title("Construct Time")
plt.plot(x,h_ct,'rs--')
plt.plot(x,b_ct,'bs--')
plt.xticks(x,xlabel)
plt.ylabel("Construct Time(seconds)")
plt.legend(['hash','binary'])
plt.grid()

plt.subplot(224)
plt.title("Correct")
plt.plot(x,h_cor,'rs--')
plt.plot(x,b_cor,'bs--')
plt.xticks(x,xlabel)
plt.ylabel("Percent ( %)")
plt.legend(['hash','binary'])
plt.grid()

plt.show()





