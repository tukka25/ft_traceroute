#!/bin/bash

# Define ANSI colors
RED='\033[1;31m'
BLUE='\033[1;34m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Namespaces and interfaces
nameSpace1="red"
nameSpace2="blue"
routerNS="router"
vEth1="veth-red"
rVeth1="r-veth-red"
vEth2="veth-blue"
rVeth2="r-veth-blue"
ip1="192.168.10.1"
ip2="192.168.20.1"
rIP1="192.168.10.254"
rIP2="192.168.20.254"

echo -e "${YELLOW}🧹 Cleaning old namespaces...${NC}"
ip netns delete $nameSpace1 2>/dev/null
ip netns delete $nameSpace2 2>/dev/null
ip netns delete $routerNS 2>/dev/null
sleep 1

echo -e "${GREEN}🧠 Creating namespaces...${NC}"
ip netns add $nameSpace1
ip netns add $nameSpace2
ip netns add $routerNS
sleep 1

echo -e "${GREEN}🔌 Creating veth pairs...${NC}"
ip link add $vEth1 type veth peer name $rVeth1
ip link add $vEth2 type veth peer name $rVeth2
sleep 1

echo -e "${GREEN}📦 Assigning veth interfaces to namespaces...${NC}"
ip link set $vEth1 netns $nameSpace1
ip link set $rVeth1 netns $routerNS
ip link set $vEth2 netns $nameSpace2
ip link set $rVeth2 netns $routerNS
sleep 1

echo -e "${GREEN}🌐 Configuring IP addresses...${NC}"
ip netns exec $nameSpace1 ip addr add $ip1/24 dev $vEth1
ip netns exec $routerNS ip addr add $rIP1/24 dev $rVeth1
ip netns exec $nameSpace2 ip addr add $ip2/24 dev $vEth2
ip netns exec $routerNS ip addr add $rIP2/24 dev $rVeth2
sleep 1

echo -e "${GREEN}📶 Bringing interfaces up...${NC}"
ip netns exec $nameSpace1 ip link set $vEth1 up
ip netns exec $routerNS ip link set $rVeth1 up
ip netns exec $nameSpace2 ip link set $vEth2 up
ip netns exec $routerNS ip link set $rVeth2 up

ip netns exec $nameSpace1 ip link set lo up
ip netns exec $nameSpace2 ip link set lo up
ip netns exec $routerNS ip link set lo up
sleep 1

echo -e "${GREEN}📍 Adding default routes...${NC}"
ip netns exec $nameSpace1 ip route add default via $rIP1 dev $vEth1
ip netns exec $nameSpace2 ip route add default via $rIP2 dev $vEth2
sleep 1

echo -e "${GREEN}🚦 Enabling IP forwarding in router...${NC}"
ip netns exec $routerNS sysctl -w net.ipv4.ip_forward=1 > /dev/null
sleep 1

echo -e "${BLUE}📡 Pinging from ns1 (red) to ns2 (blue)...${NC}"
sleep 1
ip netns exec $nameSpace1 ping $ip2 -c 5

echo -e "${YELLOW}🧹 Cleaning up namespaces...${NC}"
ip netns delete $nameSpace1
ip netns delete $nameSpace2
ip netns delete $routerNS
echo -e "${GREEN}✅ Done!${NC}"
