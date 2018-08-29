#include <fcntl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>

class v4ldev {
    int fd;

public:
    v4ldev() {
        fd = open("/dev/video0", O_RDWR /* required */ | O_NONBLOCK, 0);
    }

    ~v4ldev() {
        if (fd != -1) close(fd);
    }

    bool opened() { return fd != -1; }

    int xioctl(int request, void *arg) {
        int r;
        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);
        return r;
    }

    void *map(v4l2_buffer &buf) {
        return mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    }

    int sel() {
        int r;
        fd_set fds;
        struct timeval tv = {0};

        do {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* Timeout. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);
        } while ((r == -1 && (errno == EINTR)));

//        FD_ZERO(&fds);
//        FD_SET(fd, &fds);
//        tv.tv_sec = 2;
//        r = select(fd+1, &fds, NULL, NULL, &tv);
        return r;
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    v4ldev dev;

    if (!dev.opened()) {
        std::cout << "Failed to open /dev/video" << std::endl;
        return -1;
    }

    struct v4l2_capability cap;
    if (dev.xioctl(VIDIOC_QUERYCAP, &cap) == -1) {
        if (EINVAL == errno) {
            std::cout << "/dev/video is no V4L2 device" << std::endl;
            return -1;
        }
        return -2;
    }

    if (cap.capabilities  & V4L2_CAP_VIDEO_CAPTURE) {
        std::cout << "Can capture video" << std::endl;
    }

    if (cap.capabilities  & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
        std::cout << "Can capture video mplane" << std::endl;
    }

    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 320;
    fmt.fmt.pix.height = 240;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; // V4L2_PIX_FMT_SGRBG10;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (dev.xioctl(VIDIOC_S_FMT, &fmt) == -1)
    {
        perror("Setting Pixel Format");
        return 1;
    }

    struct v4l2_requestbuffers req = {0};
    req.count = 3;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (dev.xioctl(VIDIOC_REQBUFS, &req) == -1)
    {
        perror("Requesting Buffer");
        return 1;
    }
    std::cout << "buffers requested " << req.count << std::endl;

    if (req.count > 4) {
        return 1;
    }

    void *buffers[4];
    for (int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf = {0};
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (dev.xioctl(VIDIOC_QBUF, &buf) == -1)
        {
            perror("Requesting Buffer");
            return 1;
        }
        buffers[i] = dev.map(buf);

    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(dev.xioctl(VIDIOC_STREAMON, &type) == -1)
    {
        perror("Start Capture");
        return 1;
    }

    int r = dev.sel();
    if(-1 == r)
    {
        perror("Waiting for Frame");
        return 1;
    }

    struct v4l2_buffer buf = {0};
    for (int i = 0; i < 10; i++) {
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        r = dev.xioctl(VIDIOC_DQBUF, &buf);
        if (-1 != r) break;
    }
    if(-1 == r)
    {
        perror("Retrieving Frame");
        return 1;
    }

    std::cout << "buffer " << buf.index << ", bytes " << buf.bytesused << std::endl;

    std::ofstream f;
    f.open("frame.jpg", std::ios::binary | std::ios::trunc);
    f.write((char *)buffers[buf.index], buf.bytesused);
    f.close();

    return 0;
}