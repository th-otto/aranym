/* Joy 2001 */

#include "icio.h"

class RTC : public ICio {
private:
	uae_u8 addr;
	char nvram_filename[512];

public:
	RTC();
	virtual ~RTC();
	void init(void);
	bool load(void);
	bool save(void);
	virtual uae_u8 handleRead(uaecptr);
	virtual void handleWrite(uaecptr, uae_u8);

private:
	void setAddr(uae_u8 value);
	uae_u8 getData();
	void setData(uae_u8);
	void setChecksum();
};
