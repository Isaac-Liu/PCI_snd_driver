#include <linux/init.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/initval.h>

/* constructor -- see "Constructor" sub-section */
static int __devinit snd_mychip_probe(struct pci_dev * pci,
                                      const struct pci_device_is * pci_id)
{
        static int dev;
        static snd_card * card;
        struct muychip * chip;
        int err;
        /* (1) */
        if (dev >= SNDRV_CARDS)
                return -ENODEV;
        if (!enable[dev]) {
                dev++;
                return -ENOENT;
        }
        /* (2) */
        err = snd_card_create(index[dev], id[dev], THIS_MODULE, 0, &card);
        if (err < 0)
                return err;
        /* (3) */
        err = snd_mychip_create(card, pci, &chip);
        if (err < 0) {
                snd_card_free(card);
                return err;
        }
        /* (4) */
        strcpy(card->driver, "My Chip");
        strcpy(card->shortname, "My Own Chip 123");
        sprintf(card->longname, "%s at 0x%lx irq %i",
                card->shortname, chip->ioport, chip->irq);
        /* (5) */
        //....
        /* implemented later */

        /* (6) */
        err = snd_card_register(card);
        if (err < 0) {
                snd_card_free(card);
                return err;
        }
        /* (7) */
        pci_set_drvdata(pci, card);
        dev++;
        return 0;
}
/* destructor -- see the "Destructor" sub-section */
static void __devexit snd_mychip_remove(struct pci_dev *pci)
{
        snd_card_free(pci_get_drvdata(pci));
        pci_set_drvdata(pci, NULL);
}

