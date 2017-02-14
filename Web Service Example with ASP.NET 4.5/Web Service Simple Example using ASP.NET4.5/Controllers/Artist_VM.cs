using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Mvc;

namespace Assignment9.Controllers
{
    public class ArtistAdd
    {     
        public ArtistAdd()
        {
            BirthOrStartDate = DateTime.Now;
        }
        // For an individual, can be birth name, or a stage/performer name
        // For a duo/band/group/orchestra, will typically be a stage/performer name
        [Required, StringLength(100),Display(Name ="Artist name or stage name")]
        public string Name { get; set; }

        // For an individual, a birth name
        [StringLength(100), Display(Name = "If applicable, artist's birth name")]
        public string BirthName { get; set; }

        // For an individual, a birth date
        // For all others, can be the date the artist started working together
        [DataType(DataType.Date), Display(Name = "Birth date, or start date"), DisplayFormat(DataFormatString = "{0:MMM-dd-yyyy}")]
        public DateTime BirthOrStartDate { get; set; }

        // Get from Apple iTunes Preview, Amazon, or Wikipedia
        [Required, StringLength(200), Display(Name = "Artist Photo")]
        public string UrlArtist { get; set; }

        [Required, Display(Name = "Artist's primary genre")]
        public string Genre { get; set; }

        [StringLength(2000), DataType(DataType.MultilineText), Required, Display(Name = "Artist profile")]
        public string Profile { get; set; }

        // User name who looks after this artist
        [StringLength(200)]
        public string Executive { get; set; }      
    }

    public class AddArtistForm : ArtistAdd
    {
        [Display(Name = "List of Genres")]
        public SelectList GenreList { get; set; }
    }
    public class ArtistBase : ArtistAdd
    {
        [Key]
        public int Id { get; set; }
    }

    public class ArtistWithDetails : ArtistBase
    {
        public ArtistWithDetails()
        {
            Albums = new List<AlbumBase>();
        }
        public IEnumerable<AlbumBase> Albums { get; set; }
       
    }

    public class ArtistWithMedia : ArtistBase
    {
        public ArtistWithMedia()
        {
            ArtistMedia = new List<ArtistMediaBase>();
        }
        public IEnumerable<ArtistMediaBase> ArtistMedia { get; set; }
    }
}
