import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.URL;
import java.util.ArrayList;
import java.io.File;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

public class ImageListApp {
    static class Item { String text; String imgPathOrUrl; ImageIcon icon; }
    private DefaultListModel<Item> model = new DefaultListModel<>();
    private JList<Item> jlist;

    public ImageListApp() {
        JFrame f = new JFrame("Image List App");
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setSize(800,600);
        JPanel top = new JPanel();
        JTextField txt = new JTextField(20);
        JTextField url = new JTextField(30);
        JButton fileBtn = new JButton("Choose File");
        JButton addBtn = new JButton("Add");

        top.add(new JLabel("Text:")); top.add(txt);
        top.add(new JLabel("Img URL/Path:")); top.add(url);
        top.add(fileBtn); top.add(addBtn);
        f.add(top, BorderLayout.NORTH);

        jlist = new JList<>(model);
        jlist.setCellRenderer(new ListCellRenderer<Item>(){
            public Component getListCellRendererComponent(JList<? extends Item> list, Item v, int index, boolean sel, boolean foc){
                JPanel p = new JPanel(new BorderLayout());
                JLabel limg = new JLabel();
                if(v.icon!=null) limg.setIcon(v.icon);
                else limg.setText("No image");
                p.add(limg, BorderLayout.WEST);
                p.add(new JLabel(v.text), BorderLayout.CENTER);
                if(sel) p.setBackground(Color.LIGHT_GRAY);
                return p;
            }
        });
        JScrollPane sp = new JScrollPane(jlist);
        f.add(sp, BorderLayout.CENTER);

        fileBtn.addActionListener(e->{
            JFileChooser ch = new JFileChooser();
            if(ch.showOpenDialog(f)==JFileChooser.APPROVE_OPTION){
                File sel = ch.getSelectedFile();
                url.setText(sel.getAbsolutePath());
            }
        });

        addBtn.addActionListener(e->{
            String t = txt.getText().trim();
            String u = url.getText().trim();
            if(t.isEmpty()) return;
            Item it = new Item();
            it.text = t;
            it.imgPathOrUrl = u.isEmpty() ? null : u;
            // try load image
            try {
                BufferedImage img;
                if(it.imgPathOrUrl != null && (it.imgPathOrUrl.startsWith("http://") || it.imgPathOrUrl.startsWith("https://"))){
                    img = ImageIO.read(new URL(it.imgPathOrUrl));
                } else if(it.imgPathOrUrl != null){
                    img = ImageIO.read(new File(it.imgPathOrUrl));
                } else img = null;
                if(img!=null){
                    Image scaled = img.getScaledInstance(80,80,Image.SCALE_SMOOTH);
                    it.icon = new ImageIcon(scaled);
                }
            } catch(Exception ex){ ex.printStackTrace(); }
            model.addElement(it);
            txt.setText(""); url.setText("");
        });

        f.setVisible(true);
    }

    public static void main(String[] args){ SwingUtilities.invokeLater(ImageListApp::new); }
}

