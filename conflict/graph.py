import matplotlib.pyplot as plt
import numpy as np


h_sample= []
count = -1
search_num = 0
xlabel = []
h_conf,h_hit = [],[]	# conflict
h_st = []	# searchTime
h_ct = [] # ConstructTime
h_cor= [] # Correct
h_ma= [] # Move Average
h_os = []	# One Slot
h_ts = []	# Total Slot
h_cycle = [] # Cycle

with open("log","r") as f:
	lines = f.readlines()
	for line in lines:
		if "Sample Num" in line:
			count = count + 1
			data = {}
			h_sample.append(data)
			sample_num = line.split(" ")[3]
			h_sample[count]['sample_num'] = sample_num
		elif "Search Node" in line:
			search_num = int(line.split(" ")[3])
			
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
		elif "Number of Total" in line:
			total_slot = line.split(" ")[4]
			h_sample[count]['total_slot'] = float(total_slot)
		elif "Number of Cycle" in line:
			cycle = line.split(" ")[4]
			h_sample[count]['cycle'] = int(cycle)

for data in h_sample:
	xlabel.append(data['sample_num'])
	h_conf.append(data['conflict'])
	h_st.append(data['search_time'])
	h_ct.append(data['construct_time'])	
	h_cor.append(data['correct_per'])
	h_ma.append(data['move_avg'])
	h_hit.append(data['hit'])
	h_os.append(data['one_slot'])
	h_ts.append(data['total_slot'])
	h_cycle.append(data['cycle'])

plt.figure(figsize=(12,12))

# Conflict Rate
plt.subplot(231)
x = np.arange(len(xlabel))
plt.title("Node ({})".format(search_num))
plt.bar(x,h_hit,color="orange")
plt.bar(x,h_conf,bottom=h_hit,color="red")
plt.xticks(x,xlabel)
plt.ylabel("Rate( %)")
plt.legend(['Hits','Conflicts'])
'''
# Search Time
plt.subplot(332)
plt.title("Search Time")
plt.plot(x,h_st,'s--')
plt.xticks(x,xlabel)
plt.ylabel("Search Time(seconds)")
plt.grid()

# Construct Time
plt.subplot(333)
plt.title("Construct Time")
plt.plot(x,h_ct,'rs--')
plt.xticks(x,xlabel)
plt.ylabel("Construct Time(seconds)")
plt.grid()
'''
# Search Move Average
plt.subplot(232)
plt.title("Search Move Average")
plt.plot(x,h_ma,'rs--')
plt.xticks(x,xlabel)
plt.ylabel("Time")
plt.grid()

# One Slot Percent
plt.subplot(233)
plt.title("One Slot Percent")
plt.bar(x,h_os)
plt.xticks(x,xlabel)
plt.ylabel("Percent ( %)")

# Average List Length
plt.subplot(234)
plt.title("Average List Length")
plt.bar(x,h_ts)
plt.xticks(x,xlabel)
plt.ylabel("Length")

# Cycle
plt.subplot(236)
plt.title("Cycle")
plt.plot(x,h_cycle,'s--')
plt.xticks(x,xlabel)
plt.ylabel("Cycle")
plt.grid()

plt.show()





