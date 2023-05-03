/*
 * Copyright 2021 dorkbox, llc
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package dorkbox.systemTray;

import java.awt.Image;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.InputStream;
import java.net.URL;

import javax.imageio.stream.ImageInputStream;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JMenuItem;

import dorkbox.systemTray.peer.MenuItemPeer;
import dorkbox.systemTray.util.ImageResizeUtil;
import dorkbox.util.SwingUtil;

/**
 * This represents a common menu-entry, that is cross platform in nature
 */
@SuppressWarnings({"unused", "SameParameterValue", "WeakerAccess"})
public
class MenuItem extends Entry {
    static boolean alreadyEmittedTooltipWarning = false;

    private volatile String text;
    private volatile Object unknownImage = null;
    private volatile File imageFile;
    private volatile ActionListener callback;

    // default enabled is always true
    private volatile boolean enabled = true;

    // default is 0, which will remove the shortcut key from the native peer
    private volatile char mnemonicKey = 0;
    private volatile String tooltip;

    public
    MenuItem() {
    }

    public
    MenuItem(final String text) {
        this.text = text;
    }

    public
    MenuItem(final String text, final ActionListener callback) {
        this.text = text;
        this.callback = callback;
    }

    public
    MenuItem(final String text, final String imagePath) {
        this(text, imagePath, null);
    }

    public
    MenuItem(final String text, final File imageFile) {
        this(text, imageFile, null);
    }

    public
    MenuItem(final String text, final URL imageUrl) {
        this(text, imageUrl, null);
    }

    public
    MenuItem(final String text, final InputStream imageStream) {
        this(text, imageStream, null);
    }

    public
    MenuItem(final String text, final Image image) {
        this(text, image, null);
    }

    public
    MenuItem(final String text, final String imagePath, final ActionListener callback) {
        this.text = text;
        this.unknownImage = imagePath; // imageFile is set in the 'bind' call
        this.callback = callback;
    }

    public
    MenuItem(final String text, final File imageFile, final ActionListener callback) {
        this.text = text;
        this.unknownImage = imageFile; // imageFile is set in the 'bind' call
        this.callback = callback;
    }

    public
    MenuItem(final String text, final URL imageUrl, final ActionListener callback) {
        this.text = text;
        this.unknownImage = imageUrl; // imageFile is set in the 'bind' call
        this.callback = callback;
    }

    public
    MenuItem(final String text, final InputStream inputStream, final ActionListener callback) {
        this.text = text;
        this.unknownImage = inputStream; // imageFile is set in the 'bind' call
        this.callback = callback;
    }

    public
    MenuItem(final String text, final Image image, final ActionListener callback) {
        this.text = text;
        this.unknownImage = image; // imageFile is set in the 'bind' call
        this.callback = callback;
    }

    public
    MenuItem(final String text, final ImageInputStream imageStream, final ActionListener callback) {
        this.text = text;
        this.unknownImage = imageStream; // imageFile is set in the 'bind' call
        this.callback = callback;
    }

    public
    MenuItem(final JMenuItem jMenuItem) {
        final ActionListener[] actionListeners = jMenuItem.getActionListeners();
        //noinspection Duplicates
        if (actionListeners != null) {
            if (actionListeners.length == 1) {
                setCallback(actionListeners[0]);
            } else {
                ActionListener actionListener = e->{
                    for (ActionListener actionListener1 : actionListeners) {
                        actionListener1.actionPerformed(e);
                    }
                };
                setCallback(actionListener);
            }
        }

        setEnabled(jMenuItem.isEnabled());

        Icon icon = jMenuItem.getIcon();
        if (icon != null) {
            BufferedImage bimage = new BufferedImage(icon.getIconWidth(), icon.getIconHeight(), BufferedImage.TYPE_INT_ARGB);
            setImage(bimage);
        }

        setShortcut(jMenuItem.getMnemonic());
        setText(jMenuItem.getText());
    }

    private void realizeImageFile() {
        if (this.unknownImage instanceof String) {
            this.imageFile = imageResizeUtil.shouldResizeOrCache(false, (String) this.unknownImage);
        }
        else if (this.unknownImage instanceof File) {
            this.imageFile = imageResizeUtil.shouldResizeOrCache(false, (File) this.unknownImage);
        }
        else if (this.unknownImage instanceof URL) {
            this.imageFile = imageResizeUtil.shouldResizeOrCache(false, (URL) this.unknownImage);
        }
        else if (this.unknownImage instanceof InputStream) {
            this.imageFile = imageResizeUtil.shouldResizeOrCache(false, (InputStream) this.unknownImage);
        }
        else if (this.unknownImage instanceof Image) {
            this.imageFile = imageResizeUtil.shouldResizeOrCache(false, (Image) this.unknownImage);
        }
        else if (this.unknownImage instanceof ImageInputStream) {
            this.imageFile = imageResizeUtil.shouldResizeOrCache(false, (ImageInputStream) this.unknownImage);
        }
        this.unknownImage = null;
    }

    /**
     * @param peer the platform specific implementation for all actions for this type
     * @param parent the parent of this menu, null if the parent is the system tray
     * @param imageResizeUtil the utility used to resize images. This can be Tray specific because of cache requirements
     */
    public
    void bind(final MenuItemPeer peer, Menu parent, ImageResizeUtil imageResizeUtil) {
        super.bind(peer, parent, imageResizeUtil);

        realizeImageFile();

        peer.setImage(this);
        peer.setEnabled(this);
        peer.setText(this);
        peer.setCallback(this);
        peer.setShortcut(this);
        peer.setTooltip(this);
    }

    protected
    void setImageFromTray(final File imageFile) {
        this.imageFile = imageFile;

        if (peer != null) {
            realizeImageFile();
            ((MenuItemPeer) peer).setImage(this);
        }
    }

    /**
     * Gets the File (which is the only cross-platform solution) that is assigned to this menu entry.
     * <p>
     * This file can also be a cached file, depending on how the image was assigned to this entry.
     */
    public
    File getImage() {
        return imageFile;
    }

    /**
     * Gets the callback assigned to this menu entry
     */
    public
    ActionListener getCallback() {
        return callback;
    }

    /**
     * @return true if this item is enabled, or false if it is disabled.
     */
    public
    boolean getEnabled() {
        return this.enabled;
    }

    /**
     * Enables, or disables the entry.
     */
    public
    void setEnabled(final boolean enabled) {
        this.enabled = enabled;

        if (peer != null) {
            ((MenuItemPeer) peer).setEnabled(this);
        }
    }

    /**
     * @return the text label that the menu entry has assigned
     */
    public
    String getText() {
        return text;
    }

    /**
     * Specifies the new text to set for a menu entry
     *
     * @param text the new text to set
     */
    public
    void setText(final String text) {
        this.text = text;

        if (peer != null) {
            ((MenuItemPeer) peer).setText(this);
        }
    }

    /**
     * Specifies the new image to set for a menu entry, NULL to delete the image.
     * <p>
     * This method will cache the image if it needs to be resized to fit.
     *
     * @param imageFile the file of the image to use or null
     */
    public
    void setImage(final File imageFile) {
        this.unknownImage = imageFile;

        if (peer != null) {
            realizeImageFile(); // imageResizeUtil is set in the 'bind' call (which is also when peer is assigned)
            ((MenuItemPeer) peer).setImage(this);
        }
    }

    /**
     * Specifies the new image to set for a menu entry, NULL to delete the image
     * <p>
     * This method will cache the image if it needs to be resized to fit.
     *
     * @param imagePath the full path of the image to use or null
     */
    public
    void setImage(final String imagePath) {
        this.unknownImage = imagePath;

        if (peer != null) {
            realizeImageFile(); // imageResizeUtil is set in the 'bind' call (which is also when peer is assigned)
            ((MenuItemPeer) peer).setImage(this);
        }
    }

    /**
     * Specifies the new image to set for a menu entry, NULL to delete the image
     * <p>
     * This method will cache the image if it needs to be resized to fit.
     *
     * @param imageUrl the URL of the image to use or null
     */
    public
    void setImage(final URL imageUrl) {
        this.unknownImage = imageUrl;

        if (peer != null) {
            realizeImageFile(); // imageResizeUtil is set in the 'bind' call (which is also when peer is assigned)
            ((MenuItemPeer) peer).setImage(this);
        }
    }

    /**
     * Specifies the new image to set for a menu entry, NULL to delete the image
     * <p>
     * This method will cache the image if it needs to be resized to fit.
     *
     * @param inputStream the InputStream of the image to use
     */
    public
    void setImage(final InputStream inputStream) {
        this.unknownImage = inputStream;

        if (peer != null) {
            realizeImageFile(); // imageResizeUtil is set in the 'bind' call (which is also when peer is assigned)
            ((MenuItemPeer) peer).setImage(this);
        }
    }

    /**
     * Specifies the new image to set for a menu entry, NULL to delete the image
     * <p>
     * This method will cache the image if it needs to be resized to fit.
     *
     * @param image the image of the image to use
     */
    public
    void setImage(final Image image) {
        this.unknownImage = image;

        if (peer != null) {
            realizeImageFile(); // imageResizeUtil is set in the 'bind' call (which is also when peer is assigned)
            ((MenuItemPeer) peer).setImage(this);
        }
    }

    /**
     * Specifies the new image to set for a menu entry, NULL to delete the image
     * <p>
     * This method will cache the image if it needs to be resized to fit.
     *
     * @param imageStream the ImageInputStream of the image to use
     */
    public
    void setImage(final ImageInputStream imageStream) {
        this.unknownImage = imageStream;

        if (peer != null) {
            realizeImageFile(); // imageResizeUtil is set in the 'bind' call (which is also when peer is assigned)
            ((MenuItemPeer) peer).setImage(this);
        }
    }


    /**
     * @return true if this menu entry has an image assigned to it, or is just text.
     */
    public
    boolean hasImage() {return imageFile != null;}

    /**
     * Sets a callback for a menu entry. This is the action that occurs when one clicks the menu entry
     *
     * @param callback the callback to set. If null, the callback is safely removed.
     */
    public
    void setCallback(final ActionListener callback) {
        this.callback = callback;

        if (peer != null) {
            ((MenuItemPeer) peer).setCallback(this);
        }
    }

    /**
     * Gets the shortcut key for this menu entry (Mnemonic) which is what menu entry uses to be "selected" via the keyboard while the
     * menu is displayed.
     *
     * Mnemonics are case-insensitive, and if the character defined by the mnemonic is found within the text, the first occurrence
     * of it will be underlined.
     */
    public
    char getShortcut() {
        return this.mnemonicKey;
    }

    /**
     * Sets a menu entry shortcut key (Mnemonic) so that menu entry can be "selected" via the keyboard while the menu is displayed.
     *
     * Mnemonics are case-insensitive, and if the character defined by the mnemonic is found within the text, the first occurrence
     * of it will be underlined.
     *
     * @param key this is the key to set as the mnemonic
     */
    public
    void setShortcut(final char key) {
        this.mnemonicKey = key;

        if (peer != null) {
            ((MenuItemPeer) peer).setShortcut(this);
        }
    }

    /**
     * Sets a menu entry shortcut key (Mnemonic) so that menu entry can be "selected" via the keyboard while the menu is displayed.
     *
     * Mnemonics are case-insensitive, and if the character defined by the mnemonic is found within the text, the first occurrence
     * of it will be underlined.
     *
     * @param key this is the VK key to set as the mnemonic
     */
    public
    void setShortcut(final int key) {
        this.mnemonicKey = SwingUtil.getFromVirtualKey(key);

        if (peer != null) {
            ((MenuItemPeer) peer).setShortcut(this);
        }
    }

    /**
     * Specifies the tooltip text, usually this is used to brand the SystemTray icon with your product's name, or to provide extra
     * information during mouse-over for menu entries.
     * <p>
     * NOTE: Maximum length is 64 characters long, and it is not supported on all Operating Systems and Desktop Environments.
     * <p>
     * For more details on Linux see https://bugs.launchpad.net/indicator-application/+bug/527458/comments/12.
     *
     * @param tooltipText the text to use as a mouse-over tooltip for the tray icon or menu entry, null to remove.
     */
    public
    void setTooltip(final String tooltipText) {
        if (tooltipText != null) {
            // this is a safety precaution, since the behavior of really long text is undefined.
            if (tooltipText.length() > 64) {
                throw new RuntimeException("Tooltip text cannot be longer than 64 characters.");
            }

            if (!alreadyEmittedTooltipWarning) {
                alreadyEmittedTooltipWarning = true;
                SystemTray.logger.warn("Please disable tooltips, as they are not consistent across all platforms and tray types.");
            }
        }

        this.tooltip = tooltipText;

        if (peer != null) {
            ((MenuItemPeer) peer).setTooltip(this);
        }
    }

    /**
     * Gets the mouse-over tooltip for the meme entry.
     *
     * NOTE: This is not consistent across all platforms and tray types.
     */
    public
    String getTooltip() {
        return this.tooltip;
    }

    /**
     * @return a copy of this MenuItem as a swing JMenuItem, with all elements converted to their respective swing elements.
     */
    protected
    JMenuItem asSwingComponent() {
        JMenuItem jMenuItem = new JMenuItem();

        if (getImage() != null) {
            jMenuItem.setIcon(new ImageIcon(getImage().getAbsolutePath()));
        }
        jMenuItem.setText(getText());
        jMenuItem.setToolTipText(getTooltip());
        jMenuItem.setEnabled(getEnabled());
        jMenuItem.setMnemonic(SwingUtil.getVirtualKey(getShortcut()));

        jMenuItem.addActionListener(getCallback());

        return jMenuItem;
    }
}
