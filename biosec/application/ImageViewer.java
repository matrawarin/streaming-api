import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

public class ImageViewer {

    private JFrame frame;
    private JLabel imageLabel;

    public ImageViewer() {
        frame = new JFrame("Image Viewer (Swing)");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(900, 700);

        JMenuBar mb = new JMenuBar();
        JMenu file = new JMenu("File");
        JMenuItem open = new JMenuItem("Open...");
        open.addActionListener(e -> openImage());
        file.add(open);
        mb.add(file);
        frame.setJMenuBar(mb);

        imageLabel = new JLabel("", SwingConstants.CENTER);
        JScrollPane scroll = new JScrollPane(imageLabel);
        frame.getContentPane().add(scroll, BorderLayout.CENTER);

        frame.setVisible(true);
    }

    private void openImage() {
        JFileChooser chooser = new JFileChooser();
        int ret = chooser.showOpenDialog(frame);
        if (ret == JFileChooser.APPROVE_OPTION) {
            File f = chooser.getSelectedFile();
            try {
                BufferedImage img = ImageIO.read(f);
                if (img == null) throw new Exception("Not an image");
                ImageIcon icon = new ImageIcon(img);
                imageLabel.setIcon(icon);
                frame.setTitle("Image Viewer - " + f.getName());
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(frame, "Cannot open image: " + ex.getMessage());
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(ImageViewer::new);
    }
}

