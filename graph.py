import matplotlib.pyplot as plt
import numpy as np


h_sample,b_sample = [] , []
count = -1
buff_size = 0
xlabel = []
h_conf,h_hit = [],[]	# conflict
b_st,h_st,h_ft = [],[],[]	# searchTime
h_ct,b_ct = [],[] # ConstructTime
h_cor,b_cor = [], [] # Correct
h_ma,b_ma = [], [] # Move Average
h_os = []	# One Slot
h_cycle,b_cycle = [],[] # Cycle

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
			conflict = line.split(" ")[4]
			h_sample[count]['conflict'] = float(conflict)

		elif "Search Time" in line:
			search_time = line.split(" ")[3]
			h_sample[count]['search_time'] = float(search_time)

		elif "constructing time" in line:
			construct_time = line.split(" ")[3]
			h_sample[count]['construct_time'] = float(construct_time)
		elif "Correct Percent" in line:
			correct = line.split(" ")[3]
			h_sample[count]['correct_per'] = float(correct)
		elif "Move Average" in line:
			move = line.split(" ")[3]
			h_sample[count]['move_avg'] = float(move)
		elif "Hits" in line:
			hit = line.split(" ")[4]
			h_sample[count]['hit'] = float(hit)
		elif "One Slot" in line:
			one_slot = line.split(" ")[6]
			h_sample[count]['one_slot'] = float(one_slot)
		elif "Number of Cycle" in line:
			cycle = line.split(" ")[4]
			h_sample[count]['cycle'] = int(cycle)
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
		elif "Move Average" in line:
			move = line.split(" ")[3]
			b_sample[count]['move_avg'] = float(move)
		elif "Number of Cycle" in line:
			cycle = line.split(" ")[4]
			b_sample[count]['cycle'] = int(cycle)


for data in h_sample:
	xlabel.append(data['sample_num'])
	h_conf.append(data['conflict'])
	h_st.append(data['search_time'])
	h_ct.append(data['construct_time'])	
	h_cor.append(data['correct_per'])
	h_ma.append(data['move_avg'])
	h_hit.append(data['hit'])
	h_os.append(data['one_slot'])
	h_cycle.append(data['cycle'])

for data in b_sample:
	b_st.append(data['search_time'])
	b_ct.append(data['construct_time'])
	b_cor.append(data['correct_per'])
	b_ma.append(data['move_avg'])
	b_cycle.append(data['cycle'])

plt.figure(figsize=(12,12))

# Conflict Rate
plt.subplot(331)
x = np.arange(len(xlabel))
plt.title("Buffer ({})".format(buff_size))
plt.bar(x,h_hit,color="orange")
plt.bar(x,h_conf,bottom=h_hit,color="red")
plt.xticks(x,xlabel)
plt.ylabel("Rate( %)")
plt.legend(['Hits','Conflicts'])

# Search Time
plt.subplot(332)
plt.title("Search Time")
plt.plot(x,h_st,'s--')
plt.plot(x,b_st,'s--')
plt.xticks(x,xlabel)
plt.ylabel("Search Time(seconds)")
plt.legend(['hash','binary'])
plt.grid()

# Construct Time
plt.subplot(333)
plt.title("Construct Time")
plt.plot(x,h_ct,'rs--')
plt.plot(x,b_ct,'bs--')
plt.xticks(x,xlabel)
plt.ylabel("Construct Time(seconds)")
plt.legend(['hash','binary'])
plt.grid()

# Correct Percent
plt.subplot(334)
plt.title("Correct")
plt.plot(x,h_cor,'rs--')
plt.plot(x,b_cor,'bs--')
plt.xticks(x,xlabel)
plt.ylabel("Percent ( %)")
plt.legend(['hash','binary'])
plt.grid()

# Search Move Average
plt.subplot(335)
plt.title("Search Move Average")
plt.plot(x,h_ma,'rs--')
plt.plot(x,b_ma,'bs--')
plt.xticks(x,xlabel)
plt.ylabel("Seconds")
plt.legend(['hash','binary'])
plt.grid()

# One Slot Percent
plt.subplot(336)
plt.title("One Slot Percent")
plt.bar(x,h_os)
plt.xticks(x,xlabel)
plt.ylabel("Percent ( %)")

# Cycle
plt.subplot(337)
plt.title("Cycle")
plt.plot(x,h_cycle,'s--')
plt.plot(x,b_cycle,'s--')
plt.legend(['hash','binary'])
plt.xticks(x,xlabel)
plt.ylabel("cycle")
plt.grid()

plt.show()





