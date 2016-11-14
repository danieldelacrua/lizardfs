/*
   Copyright 2017 Skytechnology sp. z o.o.

   This file is part of LizardFS.

   LizardFS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   LizardFS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with LizardFS. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/platform.h"

#include "mount/lizard_client.h"

/*
 * This file exists in order to provide unmangled names that can be easily
 * linked with dlsym() call.
 *
 * Implementations of all functions below should not throw anything,
 * as throwing exceptions in dynamically linked code is a dangerous idea.
 */

extern "C" {

int lizardfs_fs_init(LizardClient::FsInitParams &params);
void lizardfs_fs_term();
int lizardfs_lookup(LizardClient::Context ctx, LizardClient::Inode parent,
	                                 const char *name, LizardClient::EntryParam &param);
int lizardfs_mknod(LizardClient::Context ctx, LizardClient::Inode parent, const char *name, mode_t mode, dev_t rdev, LizardClient::EntryParam &param);
int lizardfs_mkdir(LizardClient::Context ctx, LizardClient::Inode parent,
	                                 const char *name, mode_t mode, LizardClient::EntryParam &entry_param);
int lizardfs_rmdir(LizardClient::Context ctx, LizardClient::Inode parent, const char *name);
int lizardfs_open(LizardClient::Context ctx, LizardClient::Inode ino, LizardClient::FileInfo* fi);
int lizardfs_opendir(LizardClient::Context ctx, LizardClient::Inode ino);
int lizardfs_release(LizardClient::Context ctx, LizardClient::Inode ino, LizardClient::FileInfo* fi);
int lizardfs_getattr(LizardClient::Context ctx, LizardClient::Inode ino, LizardClient::AttrReply &reply);

int lizardfs_releasedir(LizardClient::Context ctx, LizardClient::Inode ino);

std::pair<int, ReadCache::Result> lizardfs_read(LizardClient::Context ctx, LizardClient::Inode ino,
	                                         size_t size, off_t off, LizardClient::FileInfo* fi);

std::pair<int, std::vector<uint8_t>> lizardfs_read_special_inode(LizardClient::Context ctx,
	                    LizardClient::Inode ino, size_t size, off_t off, LizardClient::FileInfo* fi);

std::pair<int, std::vector<LizardClient::DirEntry>> lizardfs_readdir(LizardClient::Context ctx,
	                    LizardClient::Inode ino, off_t off, size_t max_entries);

std::pair<int, ssize_t> lizardfs_write(LizardClient::Context ctx, LizardClient::Inode ino, const char *buf, size_t size, off_t off, LizardClient::FileInfo* fi);
int lizardfs_flush(LizardClient::Context ctx, LizardClient::Inode ino, LizardClient::FileInfo* fi);
bool lizardfs_isSpecialInode(LizardClient::Inode ino);
int lizardfs_update_groups(LizardClient::Context &ctx);
std::pair<int, LizardClient::JobId> lizardfs_makesnapshot(LizardClient::Context ctx, LizardClient::Inode ino,
	                                                  LizardClient::Inode dst_parent,
	                                                  const std::string &dst_name,
	                                                  bool can_overwrite);
int lizardfs_getgoal(LizardClient::Context ctx, LizardClient::Inode ino, std::string &goal);
int lizardfs_setgoal(LizardClient::Context ctx, LizardClient::Inode ino, const std::string &goal_name, uint8_t smode);

} // extern "C"