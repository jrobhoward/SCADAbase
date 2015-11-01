#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>

#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/malloc.h>

#include "imageFile_key.h"

#define BUFFER_SIZE 256

static d_open_t lkey_open;
static d_close_t lkey_close;
static d_read_t lkey_read;
static d_write_t lkey_write;

static struct cdevsw  lkey_cdevsw =
{
    .d_version = D_VERSION,
    .d_open = lkey_open,
    .d_close = lkey_close,
    .d_read = lkey_read,
    .d_write = lkey_write,
    .d_name = "lkey"
};

typedef struct lkey
{
    char buffer [BUFFER_SIZE];
    int length;
} lkey_t;

static lkey_t *lkey_message;
static struct cdev *lkey_dev;

static int lkey_open(struct cdev *dev, int oflags, int devtype, struct thread *td)
{
    uprintf("opening device\n");
    return 0;
}

static int lkey_close(struct cdev *dev, int fflag, int devtype, struct thread *td)
{
    uprintf("closing device\n");
    return 0;
}

static int lkey_write(struct cdev *dev, struct uio *uio, int ioflag)
{
    int err = 0;

    err = copyin(uio->uio_iov->iov_base, lkey_message->buffer, MIN(uio->uio_iov->iov_len, BUFFER_SIZE - 1));
    if (err != 0)
    {
        uprintf("write failed\n");
        return err;
    }

    *(lkey_message->buffer + MIN(uio->uio_iov->iov_len, BUFFER_SIZE - 1)) = 0;
    lkey_message->length = MIN(uio->uio_iov->iov_len, BUFFER_SIZE - 1);

    return err;
}

static int lkey_read(struct cdev *dev, struct uio *uio, int ioflag)
{
    int err = 0;
    int amount;

    amount = MIN(uio->uio_resid, (imageFile_key_len - uio->uio_offset > 0) ? imageFile_key_len - uio->uio_offset : 0);
    err = uiomove(imageFile_key + uio->uio_offset, amount, uio);
    if (err != 0)
    {
        uprintf("read filed\n");
    }

    return err;
}

static int lkey_modevent(module_t mod __unused, int event, void *arg __unused)
{
    int err = 0;

    switch (event)
    {
        case MOD_LOAD:
            lkey_message = malloc(sizeof(lkey_t), M_TEMP, M_WAITOK);
            lkey_dev = make_dev(&lkey_cdevsw, 0, UID_ROOT, GID_WHEEL, 0600, "lkey");
            uprintf("lkey driver loaded\n");
            break;
        case MOD_UNLOAD:
            destroy_dev(lkey_dev);
            free(lkey_message, M_TEMP);
            uprintf("lkey driver unloaded\n");
            break;
        default:
            err = EOPNOTSUPP;
            break;
    }
    return (err);
}

static moduledata_t lkey_mod = {"lkey", lkey_modevent, NULL };

DECLARE_MODULE(lkey, lkey_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);