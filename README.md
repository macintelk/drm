1) drm setup for browsing i915/xe linux sources

- create a new xcodeproj and add drm_extracted
- after indexing browsing structures should be fine


2) nblue setup

use le_kexts.sh to install tgl frame+graph kexts

opencore boot args: debug=0x144 keepsyms=1 IGLogLevel=0xe

add -allow3d to load graphics
   
add you igpu to opencore DeviceProperties (e.g PciRoot(0x0)/Pci(0x2,0x0))

<img width="324" height="123" alt="Captura de ecrã 2026-07-09, às 23 04 46" src="https://github.com/user-attachments/assets/4cb031c0-cd40-4db9-859a-97787dddbd4d" />

add this properties (rename for swap icl/tgl)
AAPL,ig-platform-id 0000528A 
device-id 528A0000 
AAPL,ig-platform-id1 0000499A 
device-id1 499A0000 

nblue will load based on installed kexts and injected device-id
