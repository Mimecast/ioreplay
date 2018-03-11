// Copyright 2018 Mimecast Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "gioop.h"

status_e gioop_run(gwriter_s *w, gtask_s *t)
{
    status_e ret = SUCCESS;

    // There was already an error in the parser (parser.c) processing this
    // task! Don't process it futher.
    if (t->ret != SUCCESS) {
        Cleanup(t->ret);
    }

    generate_s *g = w->generate;

    // Get the virtual process data object from the virtual PID space and store
    // a pointer to it at t->gprocess
    generate_gprocess_by_realpid(g, t);

    // One of the open syscalls may openes a file handle succesfully
    if (Eq(t->op, "open")) {
        Cleanup(gioop_open(w, t, g));

    } else if (Eq(t->op, "openat")) {
        Cleanup(gioop_openat(w, t, g));

    } else if (Eq(t->op, "creat")) {
        Cleanup(gioop_creat(w, t, g));
    }

    // Get the virtual file descriptor of a given real fd and store a pointer
    // to it to t->vfd.
    if (t->has_fd) {
        ret = gprocess_vfd_by_realfd(t->gprocess, t);
        Cleanup_unless(SUCCESS, ret);
    }


    if (Eq(t->op, "close")) {
        Cleanup(gioop_close(w, t, g));

    } else if (Eq(t->op, "stat")) {
        Cleanup(gioop_stat(w, t, g));

    } else if (Eq(t->op, "statfs")) {
        Cleanup(gioop_statfs(w, t, g));

    } else if (Eq(t->op, "statfs64")) {
        Cleanup(gioop_statfs64(w, t, g));

    } else if (Eq(t->op, "fstat")) {
        Cleanup(gioop_fstat(w, t, g));

    } else if (Eq(t->op, "fstatat")) {
        Cleanup(gioop_fstatat(w, t, g));

    } else if (Eq(t->op, "fstatfs")) {
        Cleanup(gioop_fstatfs(w, t, g));

    } else if (Eq(t->op, "fstatfs64")) {
        Cleanup(gioop_fstatfs64(w, t, g));

    } else if (Eq(t->op, "rename")) {
        Cleanup(gioop_rename(w, t, g));

    } else if (Eq(t->op, "renameat")) {
        Cleanup(gioop_renameat(w, t, g));

    } else if (Eq(t->op, "renameat2")) {
        Cleanup(gioop_renameat2(w, t, g));

    } else if (Eq(t->op, "read")) {
        Cleanup(gioop_read(w, t, g));

    } else if (Eq(t->op, "readv")) {
        Cleanup(gioop_readv(w, t, g));

    } else if (Eq(t->op, "readahead")) {
        Cleanup(gioop_readahead(w, t, g));

    } else if (Eq(t->op, "readdir")) {
        Cleanup(gioop_readdir(w, t, g));

    } else if (Eq(t->op, "readlink")) {
        Cleanup(gioop_readlink(w, t, g));

    } else if (Eq(t->op, "readlinkat")) {
        Cleanup(gioop_readlinkat(w, t, g));

    } else if (Eq(t->op, "write")) {
        Cleanup(gioop_write(w, t, g));

    } else if (Eq(t->op, "writev")) {
        Cleanup(gioop_writev(w, t, g));

    } else if (Eq(t->op, "lseek")) {
        Cleanup(gioop_lseek(w, t, g));

    } else if (Eq(t->op, "getdents")) {
        Cleanup(gioop_getdents(w, t, g));

    } else if (Eq(t->op, "mkdir")) {
        Cleanup(gioop_mkdir(w, t, g));

    } else if (Eq(t->op, "rmdir")) {
        Cleanup(gioop_rmdir(w, t, g));

    } else if (Eq(t->op, "mkdirat")) {
        Cleanup(gioop_mkdirat(w, t, g));

    } else if (Eq(t->op, "unlink")) {
        Cleanup(gioop_unlink(w, t, g));

    } else if (Eq(t->op, "unlinkat")) {
        Cleanup(gioop_unlinkat(w, t, g));

    } else if (Eq(t->op, "lstat")) {
        Cleanup(gioop_lstat(w, t, g));

    } else if (Eq(t->op, "fsync")) {
        Cleanup(gioop_fsync(w, t, g));

    } else if (Eq(t->op, "fdatasync")) {
        Cleanup(gioop_fdatasync(w, t, g));

    } else if (Eq(t->op, "sync")) {
        Cleanup(gioop_sync(w, t, g));

    } else if (Eq(t->op, "syncfs")) {
        Cleanup(gioop_syncfs(w, t, g));

    } else if (Eq(t->op, "sync_file_range")) {
        Cleanup(gioop_sync_file_range(w, t, g));

    } else if (Eq(t->op, "fcntl")) {
        Cleanup(gioop_fcntl(w, t, g));

    } else if (Eq(t->op, "fcntl")) {
        Cleanup(gioop_fcntl(w, t, g));

    } else if (Eq(t->op, "mmap2")) {
        // Support for mmap added later

    } else if (Eq(t->op, "munmap")) {
        // Support for mmap added later

    } else if (Eq(t->op, "mremap")) {
        // Support for mmap added later

    } else if (Eq(t->op, "msync")) {
        // Support for mmap added later

    } else if (Eq(t->op, "chmod")) {
        Cleanup(gioop_chmod(w, t, g));

    } else if (Eq(t->op, "fchmodat")) {
        Cleanup(gioop_chmod(w, t, g));

    } else if (Eq(t->op, "fchmod")) {
        Cleanup(gioop_fchmod(w, t, g));

    } else if (Eq(t->op, "chown")) {
        Cleanup(gioop_chown(w, t, g));

    } else if (Eq(t->op, "chown16")) {
        Cleanup(gioop_chown(w, t, g));

    } else if (Eq(t->op, "lchown")) {
        Cleanup(gioop_lchown(w, t, g));

    } else if (Eq(t->op, "lchown16")) {
        Cleanup(gioop_lchown(w, t, g));

    } else if (Eq(t->op, "fchown")) {
        Cleanup(gioop_fchown(w, t, g));

    } else if (Eq(t->op, "fchownat")) {
        Cleanup(gioop_chown(w, t, g));

    } else if (Eq(t->op, "exit_group")) {
        Cleanup(gioop_exit_group(w, t, g));

    } else {
        Cleanup(ERROR;);
    }

cleanup:

#ifdef LOG_FILTERED
    if (ret != SUCCESS)
        t->filtered_where = __FILE__;
#endif

    t->ret = ret;
    return ret;
}

status_e gioop_open(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd || t->path == NULL || t->flags == -1) {
        return ERROR;
    }

    gprocess_create_vfd_by_realfd(t->gprocess, t, g);
    generate_vsize_by_path(g, t, NULL);

    Gioop_write(OPEN, "%ld|%s|%d|%d|open",
                t->mapped_fd, t->path, t->mode, t->flags);

    if (t->fd > 0)
        vsize_open(t->vsize, t->vfd, t->path, t->flags);

    return SUCCESS;
}

status_e gioop_openat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd || t->path == NULL || t->flags == -1) {
        return ERROR;
    }

    gprocess_create_vfd_by_realfd(t->gprocess, t, g);
    generate_vsize_by_path(g, t, NULL);
    Gioop_write(OPEN_AT, "%ld|%s|%d|%d|openat",
                t->mapped_fd,t->path, t->mode, t->flags);
    if (t->fd > 0)
        vsize_open(t->vsize, t->vfd, t->path, t->flags);

    return SUCCESS;
}

status_e gioop_creat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd || t->path == NULL || t->flags == -1) {
        return ERROR;
    }

    gprocess_create_vfd_by_realfd(t->gprocess, t, g);
    generate_vsize_by_path(g, t, NULL);

    Gioop_write(CREAT, "%ld|%s|%d|%d|creat",
                t->mapped_fd, t->path, t->mode, t->flags);
    if (t->fd > 0)
        vsize_open(t->vsize, t->vfd, t->path, t->flags);

    return SUCCESS;
}


status_e gioop_close(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(CLOSE, "%ld|%d|close", t->mapped_fd, t->status);

    if (t->status == 0)
        vsize_close(t->vsize, t->vfd);

    hmap_remove_l(t->gprocess->fd_map, t->fd);
    hmap_remove_l(t->gprocess->vfd_map, t->mapped_fd);

    if (!(rbuffer_insert(g->vfd_buffer, t->vfd)))
        vfd_destroy(t->vfd);

    return SUCCESS;
}

status_e gioop_stat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(STAT, "%s|%d|stat", t->path, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_statfs(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(STATFS, "%s|%d|statfs", t->path, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_statfs64(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(STATFS64, "%s|%d|statfs64", t->path, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_fstat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(FSTAT, "%ld|%d|fstat", t->mapped_fd, t->status);

    return SUCCESS;
}

status_e gioop_fstatat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(FSTAT_AT, "%s|%d|fstatat", t->path, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_fstatfs(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(FSTATFS, "%ld|%d|fstatfs", t->mapped_fd, t->status);

    return SUCCESS;
}

status_e gioop_fstatfs64(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(FSTATFS64, "%ld|%d|fstatfs64", t->mapped_fd, t->status);

    return SUCCESS;
}

status_e gioop_rename(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL || t->path2 == NULL ) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(RENAME, "%s|%s|%d|rename", t->path, t->path2, t->status);

    if (t->status == 0) {
        t->vsize2 = generate_vsize_by_path(g, NULL, t->path2);
        vsize_rename(t->vsize, t->vsize2, t->path, t->path2);
    }

    return SUCCESS;
}

status_e gioop_renameat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL || t->path2 == NULL ) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(RENAME_AT, "%s|%s|%d|renameat", t->path, t->path2, t->status);

    if (t->status == 0) {
        t->vsize2 = generate_vsize_by_path(g, NULL, t->path2);
        vsize_rename(t->vsize, t->vsize2, t->path, t->path2);
    }

    return SUCCESS;
}
status_e gioop_renameat2(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL || t->path2 == NULL ) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(RENAME_AT2, "%s|%s|%d|renameat2",
                t->path, t->path2, t->status);

    if (t->status == 0) {
        t->vsize2 = generate_vsize_by_path(g, NULL, t->path2);
        vsize_rename(t->vsize, t->vsize2, t->path, t->path2);
    }

    return SUCCESS;
}

status_e gioop_read(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(READ, "%ld|%ld|read", t->mapped_fd, t->bytes);

    if (t->bytes > 0)
        vsize_read(t->vsize, t->vfd, t->vfd->path, t->bytes);

    return SUCCESS;
}

status_e gioop_readv(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(READ, "%ld|%ld|readv", t->mapped_fd, t->bytes);

    if (t->bytes > 0)
        vsize_read(t->vsize, t->vfd, t->vfd->path, t->bytes);

    return SUCCESS;
}

status_e gioop_readahead(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(READAHEAD, "%ld|%ld|%ld|readahead",
                t->mapped_fd, t->offset, t->count);

    return SUCCESS;
}

status_e gioop_readdir(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(READDIR, "%ld|%d|readdir", t->mapped_fd, t->status);

    return SUCCESS;
}

status_e gioop_readlink(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(READLINK, "%s|%d|readlink", t->path, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_readlinkat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(READLINK_AT, "%s|%d|readlinkat", t->path, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_write(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(WRITE, "%ld|%ld|write", t->mapped_fd, t->bytes);

    if (t->bytes > 0)
        vsize_write(t->vsize, t->vfd, t->path, t->bytes);

    return SUCCESS;
}

status_e gioop_writev(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(WRITEV, "%ld|%ld|writev", t->mapped_fd, t->bytes);

    if (t->bytes > 0)
        vsize_write(t->vsize, t->vfd, t->path, t->bytes);

    return SUCCESS;
}

status_e gioop_lseek(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(LSEEK, "%ld|%ld|%ld|%ld|lseek",
                t->mapped_fd, t->offset, t->whence, t->bytes);

    if (t->bytes >= 0)
        vsize_seek(t->vsize, t->vfd, t->bytes);

    return SUCCESS;
}

status_e gioop_getdents(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(GETDENTS, "%ld|%ld|%ld|getdents",
                t->mapped_fd, t->count, t->bytes);

    return SUCCESS;
}

status_e gioop_mkdir(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(MKDIR, "%s|%d|%d|mkdir", t->path, t->mode, t->status);

    if (t->status == 0)
        vsize_mkdir(t->vsize, t->path);

    return SUCCESS;
}
status_e gioop_rmdir(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(MKDIR, "%s|%d|rmdir", t->path, t->status);

    if (t->status == 0)
        vsize_rmdir(t->vsize, t->path);

    return SUCCESS;
}
status_e gioop_mkdirat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(MKDIR_AT, "%s|%d|%d|mkdirat", t->path, t->mode, t->status);

    if (t->status == 0)
        vsize_mkdir(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_unlink(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(UNLINK, "%s|%d|unlink", t->path, t->status);

    if (t->status == 0)
        vsize_unlink(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_unlinkat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(UNLINK_AT, "%s|%d|unlinkat", t->path, t->status);

    if (t->status == 0)
        vsize_unlink(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_lstat(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(LSTAT, "%s|%d|lstat", t->path, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_fsync(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(FSYNC, "%ld|%d|fsync", t->mapped_fd, t->status);

    return SUCCESS;
}

status_e gioop_fdatasync(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(FDATASYNC, "%ld|%d|fdatasync", t->mapped_fd, t->status);

    return SUCCESS;
}

status_e gioop_sync(gwriter_s *w, gtask_s *t, generate_s *g)
{
    Gioop_write(SYNC, "%d|sync", t->status);

    return SUCCESS;
}

status_e gioop_syncfs(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(SYNCFS, "%ld|%d|syncfs", t->mapped_fd, t->status);

    return SUCCESS;
}

status_e gioop_sync_file_range(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(SYNC_FILE_RANGE, "%ld|%ld|%ld|%d|sync_file_range",
                t->mapped_fd, t->offset, t->bytes, t->status);

    return SUCCESS;
}

status_e gioop_fcntl(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    switch (t->F) {
    case F_GETFD:
    case F_GETFL:
    case F_SETFD:
    case F_SETFL:
        break;
    default:
        return ERROR;
        break;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(FCNTL, "%ld|%d|%d|%d|fcntl",
                t->mapped_fd, t->F, t->G, t->status);

    return SUCCESS;
}

status_e gioop_chmod(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    Gioop_write(CHMOD, "%s|%d|%d|chmod", t->path, t->mode, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_fchmod(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(FCHMOD, "%ld|%d|%d|fchmod", t->mapped_fd, t->mode, t->status);

    return SUCCESS;
}

status_e gioop_chown(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    // Hmm, maybe rename t->offset, because here it is used for the user UID
    Gioop_write(CHOWN, "%s|%ld|%d|%d|chown", t->path, t->offset, t->G, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_fchown(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (!t->has_fd) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, t->vfd->path);
    // Hmm, maybe rename t->offset, because here it is used for the user UID
    Gioop_write(FCHOWN, "%ld|%ld|%d|%d|fchown", t->mapped_fd, t->offset, t->G, t->status);

    return SUCCESS;
}

status_e gioop_lchown(gwriter_s *w, gtask_s *t, generate_s *g)
{
    if (t->path == NULL) {
        return ERROR;
    }

    generate_vsize_by_path(g, t, NULL);
    // Hmm, maybe rename t->offset, because here it is used for the user UID
    Gioop_write(LCHOWN, "%s|%ld|%d|%d|chown", t->path, t->offset, t->G, t->status);

    if (t->status == 0)
        vsize_stat(t->vsize, t->path);

    return SUCCESS;
}

status_e gioop_exit_group(gwriter_s *w, gtask_s *t, generate_s *g)
{
    // It means that the process and all its threads terminate.
    // Therefore close all file handles of that process!
    hmap_run_cb2(t->gprocess->vfd_map, gioop_close_all_vfd_cb, t);

    // Remove virtual process from pid map and destroy it
    amap_unset(g->pid_map, t->pid);
    gprocess_destroy(t->gprocess);

    return SUCCESS;
}

void gioop_close_all_vfd_cb(void *data, void *data2)
{
    gtask_s *t = data2;
    t->vfd = data;
    generate_s *g = t->generate;

    generate_vsize_by_path(g, t, t->vfd->path);
    Gioop_write(CLOSE, "%ld|%d|close on exit_group", t->vfd->mapped_fd, 0);
    vsize_close(t->vsize, t->vfd);
}

