#Install cgroups libaries
sudo apt-get install cgroup-bin libcgroup1

# Delete old group
sudo cgdelete -g memory:limited

#Create cgroup for limited memory
sudo cgcreate -g memory:limited

#Configure Cgroup
sudo cgset -r memory.limit_in_bytes=75M limited
sudo cgset -r cpuset.cpus="0" limited

