using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Mvc;

namespace Assignment9.Controllers
{
    public class AlbumAdd
    {
   
           public AlbumAdd()
        {
            ReleaseDate = DateTime.Now;
        }

        [Required, StringLength(100), Display(Name ="Album Name")]
        public string Name { get; set; }
        
        [DataType(DataType.Date), Display(Name = "Release Date"), DisplayFormat(DataFormatString = "{0:MMM-dd-yyyy}")]
        public DateTime ReleaseDate { get; set; }

        // Get from Apple iTunes Preview, Amazon, or Wikipedia
        [Required, StringLength(200), Display(Name = "Album Cover Art")]
        public string UrlAlbum { get; set; }

        [Required, Display(Name = "Album's primary genre")]
        public string Genre { get; set; }

        [StringLength(2000), DataType(DataType.MultilineText), Display(Name = "Album Description")]
        public string Description { get; set; }

        // User name who looks after the album
        [StringLength(200)]
        public string Coordinator { get; set; }

        public string ArtistName { get; set; }

        public int ArtistId { get; set; }
    }

    public class AlbumAddForm : AlbumAdd
    {
        public SelectList GenreList { get; set; }
    }

    public class AlbumBase : AlbumAdd
    {
        [Key]
        public int Id { get; set; }
    }

    public class AlbumWithDetails : AlbumBase
    {
        public AlbumWithDetails()
        {
            Artists = new List<ArtistBase>();
            Tracks = new List<TrackBase>();
        }
        public IEnumerable<ArtistBase> Artists { get; set; }

        public IEnumerable<TrackBase> Tracks { get; set; }
    }
}
