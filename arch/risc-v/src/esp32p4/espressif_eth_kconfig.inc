config ESPRESSIF_EMAC
	bool "Ethernet MAC"
	depends on ARCH_CHIP_ESP32P4
	default n
	select NET
	select ARCH_PHY_INTERRUPT
	select ESPRESSIF_HR_TIMER
	select SCHED_LPWORK
	---help---
		Enable Ethernet MAC support.

menu "Ethernet Configuration"
	depends on ESPRESSIF_EMAC

config ESPRESSIF_ETH_NRXDESC
	int "RX descriptor number"
	default 20
	---help---
		Number of DMA receive descriptors/buffers used by the EMAC.
		Each buffer is CONFIG_ESPRESSIF_ETH_DMA_BUFFER_SIZE bytes.
		The default of 20 provides headroom for TCP-style ACK bursts
		that arrive while the upper layer is busy submitting a TX burst
		(otherwise the EMAC asserts RX_BUFF_UNAVAILABLE and silently
		drops incoming frames, forcing TCP retransmissions).

config ESPRESSIF_ETH_NTXDESC
	int "TX descriptor number"
	default 20
	---help---
		Number of DMA transmit descriptors/buffers used by the EMAC.
		With CONFIG_ESPRESSIF_ETH_DMA_BUFFER_SIZE=1600, each descriptor
		holds one Ethernet frame.  TCP send() bursts up to ~12 segments
		back-to-back; 20 descriptors gives ample headroom so that
		esp_eth_transmit() never returns ESP_ERR_NO_MEM mid-burst.
		If the ring is exhausted, frames are silently dropped and
		retransmissions happen via fast-retransmit/RTO, drastically
		degrading throughput.

config ESPRESSIF_ETH_DMA_BUFFER_SIZE
	int "DMA buffer size (bytes)"
	default 1600
	range 256 1600
	---help---
		Size of each DMA buffer used by the EMAC (must match esp_eth's
		CONFIG_ETH_DMA_BUFFER_SIZE).  At the maximum 1600 bytes one
		buffer holds an entire 1518-byte Ethernet frame, so a typical
		TX uses exactly one descriptor.  Lower values cause the MAC DMA
		to chain multiple descriptors per frame, reducing the effective
		number of frames the ring can hold.

config ESPRESSIF_ETH_TX_MAX_RETRIES
	int "TX submit retry budget"
	default 32
	range 1 256
	---help---
		Number of times emac_transmit() retries esp_eth_transmit() when
		the DMA TX descriptor ring is fully owned by hardware
		(ESP_ERR_NO_MEM).  Between attempts the driver yields the CPU
		via sched_yield() so that the RX worker (and any other peer at
		the same priority) can run while the EMAC DMA drains its TX
		descriptors in the background.  The default of 32 covers a
		full ring rotation under sustained TCP bursts while remaining
		bounded; increase if the network has very high latency between
		TX submissions and DMA descriptor recycling.

config ESPRESSIF_ETH_PHY_ADDR
	int "PHY address"
	default 1
	range 0 31
	---help---
		Address of the external Ethernet PHY on the SMI (MDC/MDIO) bus.

config ESPRESSIF_ETH_MDC_GPIO
	int "MDC GPIO"
	default 31 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the PHY MDC pin.  The shipped default
		corresponds to the ESP32-P4 Function EV Board V1.5.2; override
		for other boards/SoCs.

config ESPRESSIF_ETH_MDIO_GPIO
	int "MDIO GPIO"
	default 52 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the PHY MDIO pin.  The shipped default
		corresponds to the ESP32-P4 Function EV Board V1.5.2; override
		for other boards/SoCs.

config ESPRESSIF_ETH_RMII_CLK_GPIO
	int "RMII REF_CLK GPIO"
	default 50 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number for the RMII 50MHz reference clock (input).  The
		shipped default corresponds to the ESP32-P4 Function EV Board
		V1.5.2; override for other boards/SoCs.

config ESPRESSIF_ETH_RMII_TX_EN_GPIO
	int "RMII TX_EN GPIO"
	default 49 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the RMII TX_EN pin.  Default is for the
		ESP32-P4 Function EV Board V1.5.2.

config ESPRESSIF_ETH_RMII_TXD0_GPIO
	int "RMII TXD0 GPIO"
	default 34 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the RMII TXD0 pin.  Default is for the
		ESP32-P4 Function EV Board V1.5.2.

config ESPRESSIF_ETH_RMII_TXD1_GPIO
	int "RMII TXD1 GPIO"
	default 35 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the RMII TXD1 pin.  Default is for the
		ESP32-P4 Function EV Board V1.5.2.

config ESPRESSIF_ETH_RMII_CRS_DV_GPIO
	int "RMII CRS_DV GPIO"
	default 28 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the RMII CRS_DV pin.  Default is for
		the ESP32-P4 Function EV Board V1.5.2.

config ESPRESSIF_ETH_RMII_RXD0_GPIO
	int "RMII RXD0 GPIO"
	default 29 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the RMII RXD0 pin.  Default is for the
		ESP32-P4 Function EV Board V1.5.2.

config ESPRESSIF_ETH_RMII_RXD1_GPIO
	int "RMII RXD1 GPIO"
	default 30 if ARCH_CHIP_ESP32P4
	default -1
	---help---
		GPIO number connected to the RMII RXD1 pin.  Default is for the
		ESP32-P4 Function EV Board V1.5.2.

config ESPRESSIF_ETH_ENABLE_PHY_RSTPIN
	bool "Enable PHY reset pin"
	default y

config ESPRESSIF_ETH_PHY_RST_GPIO
	int "PHY reset GPIO"
	default 51 if ARCH_CHIP_ESP32P4
	default -1
	depends on ESPRESSIF_ETH_ENABLE_PHY_RSTPIN
	---help---
		GPIO number connected to the PHY reset pin.  Default is for the
		ESP32-P4 Function EV Board V1.5.2.

endmenu # Ethernet Configuration
