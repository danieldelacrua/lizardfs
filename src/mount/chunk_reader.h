#ifndef LIZARDFS_MFSMOUNT_CHUNK_READER_H_
#define LIZARDFS_MFSMOUNT_CHUNK_READER_H_

#include <poll.h>
#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <mutex>

#include "common/connection_pool.h"
#include "common/massert.h"
#include "common/network_address.h"
#include "mount/chunk_connector.h"
#include "mount/chunk_locator.h"
#include "mount/read_operation_planner.h"

class ChunkReader {
public:
	ChunkReader(ChunkConnector& connector, ChunkLocator& locator, ConnectionPool& connectionPool);

	/*
	 * Uses locator to locate the chunk and chooses chunkservers to read from
	 */
	void prepareReadingChunk(uint32_t inode, uint32_t index);

	/*
	 * The same as prepareReadingChunk, but is a noop if called with the same arguments
	 * more than once in a row
	 */
	void prepareReadingChunkIfNeeded(uint32_t inode, uint32_t index);

	/*
	 * Reads data from the previously located chunk and appends it to the buffer
	 */
	uint32_t readData(std::vector<uint8_t>& buffer, uint32_t offset, uint32_t size);

private:
	ConnectionPool& connectionPool_;
	ChunkConnector& connector_;
	ChunkLocator& locator_;
	ReadOperationPlanner planner_;
	std::map<ChunkType, NetworkAddress> chunkTypeLocations_;
};

#endif /* CHUNK_READER_H_ */
