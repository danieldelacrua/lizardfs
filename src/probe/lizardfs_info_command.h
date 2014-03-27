#pragma once

#include "probe/lizardfs_probe_command.h"

class LizardFsInfoCommand : public LizardFsProbeCommand {
public:
	virtual std::string name() const;
	virtual SupportedOptions supportedOptions() const;
	virtual void usage() const;
	virtual void run(const Options& options) const;
};