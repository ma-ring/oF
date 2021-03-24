using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.IO;
using System.Net.Sockets;

namespace VideoController
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string path = @"data\video.mp4";
        private string mRemoteIP = "127.0.0.1";
        private int mRemotePort = 5000;
        private UdpClient mSendClient;
        private System.Windows.Threading.DispatcherTimer mTimerSend;
        private MediaPlayer mPlayer;
        private double totalVideoSecond = 0;
        private TimeSpan mCurrentPlayerPos;
        private Boolean enableFrameShift = false;

        public MainWindow()
        {
            SetupClient();

            InitializeComponent();

            mPlayer = new MediaPlayer();
            mPlayer.ScrubbingEnabled = true;
            mPlayer.Open(new Uri(path, UriKind.Relative));
            MediaVideo.Source = new Uri(path, UriKind.Relative);

            play();

        }

        private void SetupClient()
        {
            System.Net.IPAddress ip = System.Net.IPAddress.Parse(mRemoteIP);
            mSendClient = new UdpClient();

            StartSending();
        }

        
        private void StartSending()
        {
            //send timer
            mTimerSend = new System.Windows.Threading.DispatcherTimer();
            mTimerSend.Interval = new TimeSpan(0, 0, 0, 0, 50);
            mTimerSend.Tick += (object sender, EventArgs e) => {
                sendFrame();
            };
            mTimerSend.Start();
        }
        

        private void StopSending()
        {
            //send timer
            mTimerSend.Stop();

        }

        void play()
        {
          if(MediaVideo.IsLoaded){
                MediaVideo.Play();
                mPlayer.Play();
          }
        }
        void stop()
        {

            if (MediaVideo.IsLoaded)
            {
                MediaVideo.Pause();
                mPlayer.Pause();
            }
        }
        void setPosition(int position)
        {
            if (MediaVideo.IsLoaded && !enableFrameShift)
            {
                
                int rate = (int)(totalVideoSecond * position * 0.01);

                TimeSpan ts = new TimeSpan(0, 0, 0, rate);
                MediaVideo.Position = ts;
                mPlayer.Position = ts;
            }
        }
        void setFrame(int diffFrame)
        {
            if (MediaVideo.IsLoaded)
            {
                //int diffSec = (int)(totalVideoSecond * (diffFrame /  (totalVideoSecond * 30))) ;
                int diffSec = 1000 * diffFrame / 30;

                TimeSpan ts = mCurrentPlayerPos + new TimeSpan(0, 0, 0, 0,  diffSec);
                MediaVideo.Position = ts;
                mPlayer.Position = ts;
            }
        }
        void SeekToMediaPosition(object sender, RoutedPropertyChangedEventArgs<double> args)
        {
            int time = (int)seekBar.Value;

            setPosition(time);
        }
        void ClickStart(object sender, RoutedEventArgs e)
        {
            play();
        }
        void ClickStop(object sender, RoutedEventArgs e)
        {
            stop();
        }
        void sendFrame()
        {

            int width = (int)(MediaVideo.Width);
            int height = (int)(MediaVideo.Height);

            // 描画用の Visual を用意
            var visual = new DrawingVisual();

            using (var context = visual.RenderOpen())
            {
                context.DrawVideo(mPlayer, new System.Windows.Rect(0, 0, width, height));
            }

            var bitmap = new RenderTargetBitmap(width, height, 96, 96, PixelFormats.Pbgra32);
            bitmap.Render(visual);
            //image.Source = bitmap;
            var encoder = new JpegBitmapEncoder();
            //encoder.QualityLevel = 30;
            encoder.Frames.Add(BitmapFrame.Create(bitmap));
            

            using (MemoryStream ms = new MemoryStream())
            {
                encoder.Save(ms);
                byte[] data = ms.ToArray();
                Debug.Print(data.ToString());
                mSendClient.Send(data, data.Length, mRemoteIP, mRemotePort);
            }

            //var directorypath = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "Screendumps");
            //using (var stream = new FileStream(System.IO.Path.Combine(directorypath, "test.jpg"), FileMode.Create))
            //{
            //    encoder.Save(stream);
            //    stream.Close();
            //}

        }




        protected virtual void onClose(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(mSendClient != null)
            {
                mSendClient.Close();
            }
           
            if (mTimerSend != null)
            {
                mTimerSend.Stop();
            }
        }

        private void MediaVideo_Loaded(object sender, RoutedEventArgs e)
        {
            var nd = MediaVideo.NaturalDuration;
            while (!nd.HasTimeSpan)
            {

            }
            totalVideoSecond = nd.TimeSpan.TotalSeconds;
            
        }
    }
}
