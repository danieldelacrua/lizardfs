#include "probe/list_chunkservers_command.h"

#include <iostream>
#include <vector>

#include "common/human_readable_format.h"
#include "common/lizardfs_version.h"
#include "probe/server_connection.h"

std::string ListChunkserversCommand::name() const {
	return "list-chunkservers";
}

LizardFsProbeCommand::SupportedOptions ListChunkserversCommand::supportedOptions() const {
	return {
		{kPorcelainMode, "This argument makes the output parsing-friendly."},
	};
}

void ListChunkserversCommand::usage() const {
	std::cerr << name() << " <master ip> <master port>\n";
	std::cerr << "    Prints information about all connected chunkservers.\n";
}

void ListChunkserversCommand::run(const Options& options) const {
	if (options.arguments().size() != 2) {
		throw WrongUsageException("Expected <master ip> and <master port> for " + name());
	}

	std::vector<ChunkserverEntry> chunkservers =
			getChunkserversList(options.arguments(0), options.arguments(1));

	if (!options.isSet(kPorcelainMode)) {
		std::cout << "address\tversion\tchunks\tspace\tchunks to del\tto delete\terrors"
				<< std::endl;
	}
	for (const ChunkserverEntry& cs : chunkservers) {
		if (options.isSet(kPorcelainMode)) {
			std::cout << cs.address.toString()
					<< ' ' << lizardfsVersionToString(cs.version)
					<< ' ' << cs.chunks
					<< ' ' << cs.usedSpace
					<< ' ' << cs.totalSpace
					<< ' ' << cs.tdChunks
					<< ' ' << cs.tdUsedSpace
					<< ' ' << cs.tdTotalSpace
					<< ' ' << cs.errorCount << std::endl;
		} else {
			std::cout << cs.address.toString()
					<< '\t' << lizardfsVersionToString(cs.version)
					<< '\t' << convertToSi(cs.chunks)
					<< '\t' << convertToIec(cs.usedSpace)
					<< '/' << convertToIec(cs.totalSpace)
					<< '\t' << convertToSi(cs.tdChunks)
					<< '\t' << convertToIec(cs.tdUsedSpace)
					<< '/' << convertToIec(cs.tdTotalSpace)
					<< '\t' << convertToSi(cs.errorCount) << std::endl;
		}
	}
}

std::vector<ChunkserverEntry> ListChunkserversCommand::getChunkserversList (
		const std::string& masterHost, const std::string& masterPort) {
	ServerConnection connection(masterHost, masterPort);
	std::vector<uint8_t> request, response;
	serializeMooseFsPacket(request, CLTOMA_CSERV_LIST);
	response = connection.sendAndReceive(request, MATOCL_CSERV_LIST);
	std::vector<ChunkserverEntry> result;
	while (!response.empty()) {
		result.push_back(ChunkserverEntry());
		deserialize(response, result.back());
		response.erase(response.begin(), response.begin() + serializedSize(result.back()));
	}
	return result;
}
